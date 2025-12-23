#pragma once
#include <windows.h>
#include <iostream>
#include <core/HIBIWindow.hpp>
#include <html/HTMLElement.hpp>
#include <html/p.hpp>

static const std::wstring DEFAULT_FONT = L"Segoe UI";
static const std::wstring DEFAULT_CODE_FONT = L"Consolas";

class HIRender
{

public:
    HIRender() {};

    void DrawTreeRecursive(HDC hdc, const std::vector<HTMLElement> &elements,
                           RECT rect, int &currentY);
                           
    static HFONT ElabFont(int cHeight, int cWeight, DWORD bItalic,
                    DWORD bUnderline, DWORD bStrikeOut,
                    DWORD iQuality, DWORD iPitchAndFamily, LPCWSTR pszFaceName);
    
private:

};