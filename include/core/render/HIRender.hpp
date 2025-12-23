#pragma once
#include <windows.h>
#include <iostream>
#include <core/HIBIWindow.hpp>
#include <html/HTMLElement.hpp>

class HIRender
{

public:
    HIRender() {};

    void DrawTreeRecursive(HDC hdc, const std::vector<HTMLElement> &elements,
                           RECT rect, int &currentY);

private:
    HFONT ElabFont(int cHeight, int cWeight, DWORD bItalic,
                   DWORD bUnderline, DWORD bStrikeOut,
                   DWORD iQuality, DWORD iPitchAndFamily, LPCWSTR pszFaceName);

    const std::wstring DEFAULT_FONT = L"Segoe UI";
    const std::wstring DEFAULT_CODE_FONT = L"Consolas";
};