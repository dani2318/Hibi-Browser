#include <core/render/HIRender.hpp>

HFONT HIRender::ElabFont(int cHeight, int cWeight, DWORD bItalic,
               DWORD bUnderline, DWORD bStrikeOut,
               DWORD iQuality, DWORD iPitchAndFamily, LPCWSTR pszFaceName)
{
    return CreateFontW(cHeight, 0, 0, 0, cWeight, bItalic, bUnderline, bStrikeOut,
                       DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                       iQuality, iPitchAndFamily, pszFaceName);
}

void HIRender::DrawTreeRecursive(HDC hdc, const std::vector<HTMLElement> &elements,
                                 RECT rect, int &currentY)
{
    static int recursionDepth = 0;
    if (recursionDepth > 100)
    {
        std::wcerr << L"Warning: Maximum recursion depth reached" << std::endl;
        return;
    }
    recursionDepth++;

    for (const auto &elem : elements)
    {

        std::wcout << "Formatting DOM element of type: " << elem.GetTag() << std::endl;
        if (elem.GetTag() == L"style" || elem.GetTag() == L"doctype")
        {
            std::wcout << "Found: " << elem.GetTag() << std::endl;
            continue;
        }

        if (!elem.GetContent().empty())
        {
            RECT textRect = rect;
            textRect.top = currentY;
            textRect.left += 20;
            textRect.right -= 20;

            if (textRect.right <= textRect.left || textRect.bottom <= textRect.top)
            {
                continue;
            }

            HFONT hFont = CreateFontW(-16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                      DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                      CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT.c_str());
            COLORREF textColor = RGB(0, 0, 0);

            if (elem.GetTag() == L"h1")
            {
                hFont = CreateFontW(-32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT.c_str());
            }
            else if (elem.GetTag() == L"h2")
            {
                hFont = CreateFontW(-24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT.c_str());
            }
            else if (elem.GetTag() == L"h3")
            {
                hFont = CreateFontW(-20, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT.c_str());
            }
            else if (elem.GetTag() == L"a")
            {
                hFont = CreateFontW(-16, 0, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT.c_str());
                textColor = RGB(0, 102, 204);
            }
            else if (elem.GetTag() == L"strong" || elem.GetTag() == L"b")
            {
                hFont = CreateFontW(-16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT.c_str());
            }
            else if (elem.GetTag() == L"em" || elem.GetTag() == L"i")
            {
                hFont = CreateFontW(-16, 0, 0, 0, FW_NORMAL, TRUE, FALSE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT.c_str());
            }
            else if (elem.GetTag() == L"code")
            {
                hFont = CreateFontW(-14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN, DEFAULT_CODE_FONT.c_str());
                textColor = RGB(139, 0, 0);
            }
            else if (elem.GetTag() == L"text")
            {
                hFont = CreateFontW(-16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                    CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT.c_str());
            }

            if (hFont)
            {
                HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
                SetTextColor(hdc, textColor);

                RECT calcRect = textRect;
                int height = DrawTextW(hdc, elem.GetContent().c_str(), -1, &calcRect,
                                       DT_LEFT | DT_WORDBREAK | DT_CALCRECT);

                if (height > 0)
                {
                    DrawTextW(hdc, elem.GetContent().c_str(), -1, &textRect,
                              DT_LEFT | DT_WORDBREAK);
                    currentY += (calcRect.bottom - calcRect.top) + 5;
                }

                SelectObject(hdc, hOldFont);
                DeleteObject(hFont);

                if (elem.GetTag().find(L"h") == 0 && elem.GetTag().length() == 2)
                {
                    currentY += 10;
                }
            }
        }

        if (!elem.children.empty())
        {
            DrawTreeRecursive(hdc, elem.children, rect, currentY);
        }
    }

    recursionDepth--;
}