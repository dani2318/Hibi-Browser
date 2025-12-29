#include <core/HIBIWindow.hpp>
#include <core/render/HIRender.hpp>
#include <core/parser/HTMLParser.hpp>
#include <html/HTMLElement.hpp>
#include <tchar.h>

WCHAR windowClass[] = L"HIBIWindow";

HIRender* PageRenderer = new HIRender();

void HIBIWindow::SetContent(const std::string &rawHtml)
{
    if (rawHtml.empty())
        return;

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &rawHtml[0], (int)rawHtml.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &rawHtml[0], (int)rawHtml.size(), &wstrTo[0], size_needed);

    pageContent = wstrTo;

    if (htmlParser)
    {
        try
        {
            cachedElements = htmlParser->parseHtmlToTree(pageContent);
        }
        catch (const std::exception &e)
        {
            std::wcerr << L"Error parsing HTML: " << e.what() << std::endl;
            cachedElements.clear();
        }
    }

    if (hwnd)
    {
        InvalidateRect(hwnd, NULL, TRUE);
    }
}


void HIBIWindow::PaintWindow(HIBIWindow *pWindow, const std::vector<std::shared_ptr<HTMLElement>> &elements)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    SetBkMode(hdc, TRANSPARENT);
    FillRect(hdc, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    if (!elements.empty())
    {
        int currentY = clientRect.top + 20;

        std::vector<std::shared_ptr<HTMLElement>> sharedElements;
        for (const auto &element : elements)
        {
            sharedElements.push_back(element);
        }
        PageRenderer->DrawTreeRecursive(hdc, sharedElements, clientRect, currentY);
    }

    EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HIBIWindow *pWindow = reinterpret_cast<HIBIWindow *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    switch (uMsg)
    {
    case WM_NCCREATE:
    {
        CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
        pWindow = reinterpret_cast<HIBIWindow *>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pWindow);
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    case WM_PAINT:
    {
        if (pWindow && pWindow->hwnd)
        {
            pWindow->PaintWindow(pWindow, pWindow->cachedElements);
        }
        return 0;
    }

    case WM_SIZE:
    {
        if (pWindow && pWindow->hwnd)
        {
            InvalidateRect(hwnd, NULL, TRUE);
        }
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

HIBIWindow::HIBIWindow(WindowParams p, HINSTANCE hInst, HINSTANCE hPrevInst,
                       PWSTR pCmdLine, int nCmdShow)
    : params(p),
      hInst(hInst),
      hPrevInst(hPrevInst),
      pCmdLine(pCmdLine),
      nCmdShow(nCmdShow),
      hwnd(NULL),
      pageContent(L""),
      htmlParser(nullptr)
{
    ZeroMemory(&wcex, sizeof(WNDCLASSEXW));

    const char *title = p.title.c_str();
    WCHAR windowTitle[256];
    MultiByteToWideChar(CP_UTF8, 0, title, -1, windowTitle, 256);

    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInst;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = windowClass;
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassExW(&wcex))
    {
        DWORD error = GetLastError();
        if (error != ERROR_CLASS_ALREADY_EXISTS)
        {
            MessageBox(NULL,
                       _T("Call to RegisterClassEx failed!"),
                       _T("Error"),
                       NULL);
            return;
        }
    }

    hwnd = CreateWindowExW(
        0,
        windowClass,
        windowTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL,
        NULL,
        hInst,
        this);

    if (!hwnd)
    {
        MessageBox(NULL,
                   _T("Call to CreateWindowEx failed!"),
                   _T("Error"),
                   NULL);
        return;
    }

    htmlParser = new HTMLParser(&pageContent);
}

HIBIWindow::~HIBIWindow()
{
    if (htmlParser)
    {
        delete htmlParser;
        htmlParser = nullptr;
    }
}

int HIBIWindow::run()
{
    if (!hwnd)
    {
        return -1;
    }

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