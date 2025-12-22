#include <core/HIBIWindow.hpp>
#include <tchar.h>
#include <regex>
#include <vector>
#include <string>
#include <iostream>
#include <map>

WCHAR windowClass[] = L"HIBIWindow";

struct TaggedContent
{
    std::wstring tag;
    std::wstring content;
    std::map<std::wstring, std::wstring> attributes;
};

// Versione migliorata che gestisce anche tag self-closing
std::vector<TaggedContent> splitByTags(const std::wstring &html)
{
    std::vector<TaggedContent> elements;

    // Regex migliorato che gestisce:
    // 1. Tag con contenuto: <tag>content</tag>
    // 2. Tag self-closing: <tag />
    // 3. Tag vuoti: <br>, <meta>, <!doctype>
    // 4. Testo normale
    std::wregex tagRegex(
        LR"(<!?(\w+)([^>]*)>(?:([\s\S]*?)<\/\1>)?|([^<]+))",
        std::regex_constants::icase);

    auto begin = std::wsregex_iterator(html.begin(), html.end(), tagRegex);
    auto end = std::wsregex_iterator();

    for (auto it = begin; it != end; ++it)
    {
        std::wsmatch match = *it;
        TaggedContent tc;

        if (match[1].matched)
        {
            // Tag HTML trovato
            tc.tag = match[1].str();

            // Ignora tag che non vogliamo renderizzare
            std::wstring tagLower = tc.tag;
            std::transform(tagLower.begin(), tagLower.end(), tagLower.begin(), ::towlower);

            if (tagLower == L"doctype" ||
                tagLower == L"html" ||
                tagLower == L"head" ||
                tagLower == L"meta" ||
                tagLower == L"title" ||
                tagLower == L"style" ||
                tagLower == L"script" ||
                tagLower == L"body")
            {
                // Skippa questi tag ma processa il loro contenuto
                if (match[3].matched && !match[3].str().empty())
                {
                    // Processa ricorsivamente il contenuto
                    auto innerElements = splitByTags(match[3].str());
                    elements.insert(elements.end(), innerElements.begin(), innerElements.end());
                }
                continue;
            }

            // Contenuto del tag (se presente)
            tc.content = match[3].matched ? match[3].str() : L"";
            elements.push_back(tc);
        }
        else if (match[4].matched)
        {
            // Testo normale
            std::wstring text = match[4].str();

            // Rimuovi whitespace eccessivo
            text = std::regex_replace(text, std::wregex(L"\\s+"), std::wstring(L" "));

            // Trim
            size_t start = text.find_first_not_of(L" \t\n\r");
            size_t end = text.find_last_not_of(L" \t\n\r");

            if (start != std::wstring::npos && end != std::wstring::npos)
            {
                text = text.substr(start, end - start + 1);

                if (!text.empty())
                {
                    tc.tag = L"text";
                    tc.content = text;
                    elements.push_back(tc);
                }
            }
        }
    }

    return elements;
}

void HIBIWindow::SetContent(const std::string &rawHtml)
{
    if (rawHtml.empty())
        return;
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &rawHtml[0], (int)rawHtml.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &rawHtml[0], (int)rawHtml.size(), &wstrTo[0], size_needed);
    pageContent = wstrTo;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    // 1. GET POINTER TO YOUR CLASS
    HIBIWindow *pWindow = reinterpret_cast<HIBIWindow *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (uMsg)
    {
    // 2. SETUP POINTER (Happens once when window is created)
    case WM_NCCREATE:
    {
        CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
        pWindow = reinterpret_cast<HIBIWindow *>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pWindow);
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    // 3. PAINT THE TEXT
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        if (pWindow != nullptr && !pWindow->pageContent.empty())
        {
            RECT rect;
            GetClientRect(hwnd, &rect);
            SetBkMode(hdc, TRANSPARENT);

            // Sfondo bianco
            FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

            auto elements = splitByTags(pWindow->pageContent);

            int yOffset = rect.top + 20;

            for (const auto &elem : elements)
            {
                if (elem.content.empty())
                    continue;

                RECT textRect = rect;
                textRect.top = yOffset;
                textRect.left += 20;
                textRect.right -= 20;

                HFONT hFont = nullptr;

                if (!(elem.tag == L"style") || !(elem.tag == L"script"))
                {
                    std::cout << "Current tag: ";
                    std::cout << std::string(elem.tag.begin(), elem.tag.end()) << std::endl;
                    if (elem.tag == L"h1")
                    {
                        hFont = CreateFontW(-24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
                    }
                    else if (elem.tag == L"p" || elem.tag == L"div")
                    {
                        hFont = CreateFontW(-16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
                    }
                    else if (elem.tag == L"a")
                    {
                        hFont = CreateFontW(-16, 0, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE,
                                            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
                        SetTextColor(hdc, RGB(51, 68, 136)); // Colore link
                    }
                    else
                    {
                        hFont = CreateFontW(-16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
                    }

                    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

                    DrawTextW(hdc, elem.content.c_str(),
                              static_cast<int>(elem.content.length()),
                              &textRect, DT_LEFT | DT_WORDBREAK | DT_CALCRECT);
                    DrawTextW(hdc, elem.content.c_str(),
                              static_cast<int>(elem.content.length()),
                              &textRect, DT_LEFT | DT_WORDBREAK);

                    SelectObject(hdc, hOldFont);
                    DeleteObject(hFont);

                    // Reset colore per il prossimo elemento
                    SetTextColor(hdc, RGB(0, 0, 0));

                    yOffset = textRect.bottom + 10;
                }
            }
        }
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

HIBIWindow::HIBIWindow(WindowParams p, HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR pCmdLine, int nCmdShow)
    : params(p),
      hInst(hInst),
      hPrevInst(hPrevInst),
      pCmdLine(pCmdLine),
      nCmdShow(nCmdShow)
{
    const char *title = p.title.c_str();
    WCHAR windowTitle[256];
    MultiByteToWideChar(CP_UTF8, 0, title, -1, windowTitle, 256);
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = windowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    if (!RegisterClassExW(&wcex))
    {
        MessageBox(NULL,
                   _T("Call to RegisterClassEx failed!"),
                   _T("Error"),
                   NULL);

        return;
    }
    hwnd = CreateWindowExW(
        0,                                                          // Extended window style
        windowClass,                                                // Class name
        windowTitle,                                                // Window name
        WS_OVERLAPPEDWINDOW,                                        // Style
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, // Position and size
        NULL,                                                       // Parent window
        NULL,                                                       // Menu
        hInst,                                                      // Instance handle
        this                                                        // Additional application data
    );

    if (!hwnd)
    {
        MessageBox(NULL,
                   _T("Call to CreateWindowEx failed!"),
                   _T("Error"),
                   NULL);

        return;
    }
}

int HIBIWindow::run()
{
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}