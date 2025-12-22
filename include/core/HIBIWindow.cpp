#include <core/HIBIWindow.hpp>


WCHAR windowClass[] = L"HIBIWindow";



std::map<std::wstring, std::wstring> parseAttributes(const std::wstring& attrStr) {
    std::map<std::wstring, std::wstring> attrs;
    // Regex for: name="value" or name='value' or name=value
    std::wregex attrRegex(L"(\\w+)\\s*=\\s*[\"']([^\"']*)[\"']");
    auto attrs_begin = std::wsregex_iterator(attrStr.begin(), attrStr.end(), attrRegex);
    auto attrs_end = std::wsregex_iterator();

    for (auto i = attrs_begin; i != attrs_end; ++i) {
        std::wsmatch match = *i;
        attrs[match[1].str()] = match[2].str();
    }
    return attrs;
}

std::vector<TaggedContent> parseHtmlToTree(const std::wstring &html)
{
    // A virtual "root" to hold the top-level elements
    TaggedContent root;
    root.tag = L"root";

    // Stack to track hierarchy. Points to the current parent.
    std::stack<TaggedContent*> nodeStack;
    nodeStack.push(&root);

    std::wregex tokenRegex(L"<(/?)(\\w+)([^>]*)>|([^<]+)");

    auto begin = std::wsregex_iterator(html.begin(), html.end(), tokenRegex);
    auto end = std::wsregex_iterator();

    for (auto it = begin; it != end; ++it)
    {
        std::wsmatch match = *it;

        if (match[2].matched) 
        {
            std::wstring closingSlash = match[1].str();
            std::wstring tagName = match[2].str();
            std::wstring attrStr = match[3].str();
            
            bool isClosingTag = !closingSlash.empty();
            bool isSelfClosing = (attrStr.find(L"/") != std::wstring::npos); // Check for <tag />

            // Normalize tag name
            std::transform(tagName.begin(), tagName.end(), tagName.begin(), ::towlower);

            if (isClosingTag)
            {
                if (nodeStack.size() > 1) {
                    nodeStack.pop();
                }
            }
            else
            {
                // Found <tag>. Create new node.
                TaggedContent newNode;
                newNode.tag = tagName;
                newNode.attributes = parseAttributes(attrStr);

                // Add as child to the current top of stack
                nodeStack.top()->children.push_back(newNode);
                std::vector<std::wstring> voidTags = {L"img", L"br", L"hr", L"input", L"meta"};
                bool isVoid = std::find(voidTags.begin(), voidTags.end(), tagName) != voidTags.end();

                if (!isSelfClosing && !isVoid) {
                    nodeStack.push(&nodeStack.top()->children.back());
                }
            }
        }
        else if (match[4].matched) 
        {
            std::wstring text = match[4].str();
            
            // Clean whitespace
            text = std::regex_replace(text, std::wregex(L"\\s+"), std::wstring(L" "));
            size_t first = text.find_first_not_of(L" ");
            size_t last = text.find_last_not_of(L" ");

            if (first != std::wstring::npos && last != std::wstring::npos) {
                text = text.substr(first, (last - first + 1));
                
                if (!text.empty()) {
                    TaggedContent textNode;
                    textNode.tag = L"text";
                    textNode.text = text;
                    nodeStack.top()->children.push_back(textNode);
                }
            }
        }
    }

    return root.children;
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
void HIBIWindow::DrawTreeRecursive(HDC hdc, const std::vector<TaggedContent>& elements, RECT rect, int& currentY)
{
    for (const auto& elem : elements)
    {
        if (elem.tag == L"title") 
        {
            if (!elem.text.empty()) {
                SetWindowTextW(this->hwnd, elem.text.c_str());
            } 
            else if (!elem.children.empty() && !elem.children[0].text.empty()) {
                SetWindowTextW(this->hwnd, elem.children[0].text.c_str());
            }
            continue; 
        }

        if (elem.tag == L"head") 
        {
            // FIX: Iterate over elem.children (the children of <head>), 
            // NOT headChild.children.
            for (const auto& headChild : elem.children) 
            {
                if (headChild.tag == L"title") 
                {
                    std::wstring titleText = headChild.text;
                    if (titleText.empty() && !headChild.children.empty()) {
                        titleText = headChild.children[0].text;
                    }
                    
                    if (!titleText.empty()) {
                        SetWindowTextW(this->hwnd, titleText.c_str());
                    }
                }
            }
            continue;
        }

        // Tags to completely ignore for drawing
        //TODO: ADD STYLING, JAVASCRIPT SUPPORT AND META-DATA SUPPORT.
        if (elem.tag == L"style" || elem.tag == L"script" || 
            elem.tag == L"meta" || elem.tag == L"doctype" || elem.tag == L"link")
            continue;

        if (!elem.text.empty())
        {
            RECT textRect = rect;
            textRect.top = currentY;
            textRect.left += 20;
            textRect.right -= 20;

            HFONT hFont = nullptr;
            COLORREF textColor = RGB(0, 0, 0);
            
            // Font Selection Logic
            if (elem.tag == L"h1") {
                hFont = CreateFontW(-24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            }
            else if (elem.tag == L"a") {
                hFont = CreateFontW(-16, 0, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
                textColor = RGB(51, 68, 136);
            }
            else {
                hFont = CreateFontW(-16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
            }

            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
            SetTextColor(hdc, textColor);

            // Calculate and Draw
            RECT calcRect = textRect;
            DrawTextW(hdc, elem.text.c_str(), -1, &calcRect, DT_LEFT | DT_WORDBREAK | DT_CALCRECT);
            DrawTextW(hdc, elem.text.c_str(), -1, &textRect, DT_LEFT | DT_WORDBREAK);

            SelectObject(hdc, hOldFont);
            DeleteObject(hFont);

            currentY += (calcRect.bottom - calcRect.top) + 5; 
        }

        if (!elem.children.empty())
        {
            DrawTreeRecursive(hdc, elem.children, rect, currentY);
        }
    }
}
// Main Painting Function (Called by WM_PAINT)
void HIBIWindow::PaintWindow(HIBIWindow* pWindow, const std::vector<TaggedContent>& elements)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    if (elements.empty()) {
        EndPaint(hwnd, &ps);
        return;
    }


    // Setup Background
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    SetBkMode(hdc, TRANSPARENT);
    FillRect(hdc, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    // Initialize Y Offset
    int currentY = clientRect.top + 20;

    // Start the recursive drawing using the prepared HDC
    DrawTreeRecursive(hdc, elements, clientRect, currentY);

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
        auto elements = parseHtmlToTree(pWindow->pageContent);
        pWindow->PaintWindow(pWindow, elements);
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