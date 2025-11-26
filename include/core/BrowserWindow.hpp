#define NOMINMAX
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <mutex>
#include "core/DataType.hpp"
#include "core/Debug/Debug.hpp"
#include "core/Utils.hpp"
#include "core/HTMLParser.hpp"
#include "core/NetworkFetcher.hpp"

class BrowserWindow
{
public:
    BrowserWindow(HINSTANCE hInst, Logger *debugLog)
        : hInst_(hInst), scrollPos_(0), DebugLog(debugLog)
    {
        InitWindow();
        Fetch(L"https://example.com");
    }

    void Run()
    {
        MSG msg{};
        while (GetMessageW(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

private:
    HINSTANCE hInst_;
    HWND hWnd_;
    HWND hEdit_;
    std::wstring pageContent_;
    std::mutex mutex_;
    WNDPROC oldEditProc_;
    int scrollPos_;
    std::vector<Element> elements_;
    Logger *DebugLog;
    int maxScroll_ = 0;
    void InitWindow()
    {
        WNDCLASSW wc{};
        wc.lpfnWndProc = WndProcStatic;
        wc.hInstance = hInst_;
        wc.lpszClassName = L"HibiBrowserClass";
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        RegisterClassW(&wc);

        RECT r{0, 0, 1200, 800};
        AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);

        hWnd_ = CreateWindowExW(0, wc.lpszClassName, L"Hibi Browser",
                                WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                                r.right - r.left, r.bottom - r.top,
                                nullptr, nullptr, hInst_, this);

        hEdit_ = CreateWindowExW(0, L"EDIT", L"https://example.com",
                                 WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
                                 10, 10, 800, 25, hWnd_, (HMENU)1, hInst_, nullptr);

        oldEditProc_ = (WNDPROC)SetWindowLongPtrW(hEdit_, GWLP_WNDPROC, (LONG_PTR)EditSubclassStatic);

        ShowWindow(hWnd_, SW_SHOW);
        UpdateWindow(hWnd_);
    }

    static LRESULT CALLBACK WndProcStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        BrowserWindow *self = nullptr;
        if (msg == WM_NCCREATE)
        {
            CREATESTRUCTW *cs = (CREATESTRUCTW *)lParam;
            self = (BrowserWindow *)cs->lpCreateParams;
            SetWindowLongPtrW(hWnd, GWLP_USERDATA, (LONG_PTR)self);
        }
        else
        {
            self = (BrowserWindow *)GetWindowLongPtrW(hWnd, GWLP_USERDATA);
        }
        if (self)
            return self->WndProc(hWnd, msg, wParam, lParam);
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }

    LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_PAINT:
            OnPaint();
            return 0;

        case WM_VSCROLL:
        {
            int y = scrollPos_;
            switch (LOWORD(wParam))
            {
            case SB_LINEUP:
                y -= 20;
                break;
            case SB_LINEDOWN:
                y += 20;
                break;
            case SB_PAGEUP:
                y -= 100;
                break;
            case SB_PAGEDOWN:
                y += 100;
                break;
            case SB_THUMBTRACK:
                y = HIWORD(wParam);
                break;
            }

            // Clamp to scrollable range
            scrollPos_ = std::clamp(y, 0, maxScroll_);
            InvalidateRect(hWnd_, nullptr, TRUE);
            return 0;
        }

        case WM_LBUTTONDOWN:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            for (auto &e : elements_)
            {
                if (e.isLink)
                {
                    RECT r = e.rect;
                    OffsetRect(&r, 0, -scrollPos_);
                    if (PtInRect(&r, POINT{x, y}))
                    {
                        DebugLog->Log("Link clicked: " + ToUtf8(e.href));
                        Fetch(e.href);
                        break;
                    }
                }
            }
            return 0;
        }

        case WM_APP + 1:
            InvalidateRect(hWnd_, nullptr, TRUE);
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProcW(hWnd, msg, wParam, lParam);
        }
    }

    void OnPaint()
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd_, &ps);
        RECT rc;
        GetClientRect(hWnd_, &rc);
        FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
        rc.top += 50;

        std::wstring content;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            content = pageContent_;
        }

        HtmlParser parser(content, DebugLog);
        elements_ = parser.Parse();
        DrawElements(hdc, rc, elements_);

        EndPaint(hWnd_, &ps);
    }
    void DrawElements(HDC hdc, RECT rc, std::vector<Element> &elements)
    {
        int y = rc.top - scrollPos_;
        for (auto &e : elements)
        {
            if (e.text.empty() && !e.isImage && !e.isInput)
                continue;

            if (e.isImage)
            {
                RECT imgRect = {rc.left + 10, y, rc.right - 10, y + 100};
                FillRect(hdc, &imgRect, (HBRUSH)(COLOR_BTNFACE + 1));
                e.rect = imgRect;
                y += 110;
                continue;
            }

            if (e.isInput)
            {
                RECT inputRect = {rc.left + 10, y, rc.right - 10, y + 25};
                FillRect(hdc, &inputRect, (HBRUSH)(COLOR_WINDOW + 1));
                DrawTextW(hdc, e.inputValue.c_str(), (int)e.inputValue.size(), &inputRect,
                          DT_LEFT | DT_TOP | DT_SINGLELINE | DT_VCENTER);
                e.rect = inputRect;
                y += 30;
                continue;
            }

            HFONT hFont = CreateFontW(e.style.fontSize, 0, 0, 0,
                                      e.style.bold ? FW_BOLD : FW_NORMAL,
                                      e.style.italic ? TRUE : FALSE,
                                      FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                                      CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

            HFONT oldFont = (HFONT)SelectObject(hdc, hFont);
            SetTextColor(hdc, e.style.color);
            RECT drawRect = rc;
            drawRect.top = y;
            DrawTextW(hdc, e.text.c_str(), (int)e.text.size(), &drawRect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT);
            DrawTextW(hdc, e.text.c_str(), (int)e.text.size(), &drawRect, DT_LEFT | DT_TOP | DT_WORDBREAK);
            e.rect = drawRect;
            y = drawRect.bottom + 2;
            SelectObject(hdc, oldFont);
            DeleteObject(hFont);
        }

        // Set maximum scroll based on total content height
        maxScroll_ = std::max<int>(0, y - rc.top);

        // Update vertical scrollbar
        SCROLLINFO si{};
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
        si.nMin = 0;
        si.nMax = maxScroll_;
        si.nPage = rc.bottom - rc.top;
        si.nPos = scrollPos_;
        SetScrollInfo(hWnd_, SB_VERT, &si, TRUE);

        DebugLog->Log("Finished drawing elements, total height: " + std::to_string(y - rc.top));
    }

    static LRESULT CALLBACK EditSubclassStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        BrowserWindow *self = (BrowserWindow *)GetWindowLongPtrW(GetParent(hWnd), GWLP_USERDATA);
        if (self)
            return self->EditSubclassProc(hWnd, msg, wParam, lParam);
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }

    LRESULT EditSubclassProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (msg == WM_KEYDOWN && wParam == VK_RETURN)
        {
            wchar_t buf[2048];
            GetWindowTextW(hWnd, buf, _countof(buf));
            std::wstring url(buf);
            if (!url.empty())
            {
                DebugLog->Log("Fetching URL: " + ToUtf8(url));
                Fetch(url);
            }
            return 0;
        }
        return CallWindowProcW(oldEditProc_, hWnd, msg, wParam, lParam);
    }

    void Fetch(const std::wstring &url)
    {
        NetworkFetcher::Fetch(url, [this](const std::wstring &content)
                              {
            std::lock_guard<std::mutex> lock(mutex_);
            pageContent_ = content;

            // Reset scroll position when loading a new page
            scrollPos_ = 0;

            PostMessageW(hWnd_, WM_APP + 1, 0, 0); });
    }
};
