#include <core/render/HIRender.hpp>
#include <html/HTMLElement.hpp>
#include <html/HTMLTextElement.hpp>
#include <html/HTMLHeaders.hpp>
#include <html/HTMLParagraph.hpp>
#include <tchar.h>

HFONT HIRender::ElabFont(int cHeight, int cWeight, DWORD bItalic,
                         DWORD bUnderline, DWORD bStrikeOut,
                         DWORD iQuality, DWORD iPitchAndFamily, LPCWSTR pszFaceName)
{
    return CreateFontW(cHeight, 0, 0, 0, cWeight, bItalic, bUnderline, bStrikeOut,
                       DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                       iQuality, iPitchAndFamily, pszFaceName);
}

COLORREF textColor = RGB(0, 0, 0);

HFONT HIRender::GetFontFamily(const auto &elem)
{
    int cHeight = -16;
    int cWeight = FW_NORMAL;

    DWORD bItalic = FALSE;
    DWORD bUnderline = FALSE;
    DWORD bStrikeOut = FALSE;

    DWORD iQuality = CLEARTYPE_QUALITY;
    DWORD iPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    LPCWSTR pszFaceName = DEFAULT_FONT.c_str();

    HFONT hFont = ElabFont(-16, FW_NORMAL, FALSE, FALSE, FALSE,
                           CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT.c_str());

    // Handle elements that manage their own Font object (H1, H2, H3, P)
    if (auto telem = std::dynamic_pointer_cast<HTMLTextElement>(elem))
    {
        return telem->Font();
    }

    // Set Default Values (Applied to 'text', 'a', etc.)
    cHeight    = -16;
    cWeight    = FW_NORMAL;
    bItalic    = FALSE;
    bUnderline = FALSE;
    bStrikeOut = FALSE;
    iQuality   = CLEARTYPE_QUALITY;
    iPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

    // Apply Specific Overrides based on Tag
    const std::wstring& tag = elem->GetTag();

    if (tag == L"a")
    {
        bUnderline = TRUE;
        textColor  = RGB(0, 102, 204);
    }
    else if (tag == L"strong" || tag == L"b")
    {
        cWeight    = FW_BOLD;
        textColor  = RGB(0, 102, 204);
    }
    else if (tag == L"em" || tag == L"i")
    {
        bItalic    = TRUE;
        textColor  = RGB(0, 102, 204);
    }
    else if (tag == L"code")
    {
        cHeight         = -14;
        bItalic         = TRUE;
        iPitchAndFamily = FIXED_PITCH | FF_MODERN;
        pszFaceName     = DEFAULT_CODE_FONT.c_str();
        textColor       = RGB(139, 0, 0);
    }

    return ElabFont(cHeight, cWeight, bItalic,
                         bUnderline, bStrikeOut,
                         iQuality, iPitchAndFamily, pszFaceName);
}

void GetFontFaceName(HFONT hFont) {
    HDC hdc = GetDC(NULL);
    HGDIOBJ hOldFont = SelectObject(hdc, hFont);

    TCHAR faceName[LF_FACESIZE];
    if (GetTextFace(hdc, LF_FACESIZE, faceName) > 0) {
        _tprintf(_T("\t>>> Font Name: %s\n"), faceName);
    }

    SelectObject(hdc, hOldFont);
    ReleaseDC(NULL, hdc);
}

void HIRender::DrawTreeRecursive(HDC hdc, const std::vector<std::shared_ptr<HTMLElement>> &elements,
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

        if (!elem)
            continue;

        std::wcout << "Formatting DOM element of type: " << elem.get()->GetTag() << L" " << typeid(elem).name() << std::endl;
        if (elem->GetTag() == L"style" || elem->GetTag() == L"doctype")
            continue;

        if (!elem->GetContent().empty())
        {
            RECT textRect = rect;
            textRect.top = currentY;
            textRect.left += 20;
            textRect.right -= 20;

            if (textRect.right <= textRect.left || textRect.bottom <= textRect.top)
                continue;

            HFONT hFont = GetFontFamily(elem);

            if (hFont)
            {
                HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
                SetTextColor(hdc, textColor);

                RECT calcRect = textRect;
                int height = DrawTextW(hdc, elem->GetContent().c_str(), -1, &calcRect,
                                       DT_LEFT | DT_WORDBREAK | DT_CALCRECT);

                if (height > 0)
                {
                    DrawTextW(hdc, elem->GetContent().c_str(), -1, &textRect,
                              DT_LEFT | DT_WORDBREAK);
                    currentY += (calcRect.bottom - calcRect.top) + 5;
                }

                SelectObject(hdc, hOldFont);
                DeleteObject(hFont);

                if (elem->GetTag().find(L"h") == 0 && elem->GetTag().length() == 2)
                {
                    currentY += 10;
                }
            }

            std::wcout << "\t>>> Using Font color: R: " << GetRValue(textColor) << " G: " << GetGValue(textColor) << " B: "<< GetBValue(textColor) << std::endl;
            GetFontFaceName(hFont);
            

            textColor = RGB(0, 0, 0);
        }

        if (!elem->children.empty())
        {
            DrawTreeRecursive(hdc, elem->children, rect, currentY);
        }
    }

    recursionDepth--;
}