#pragma once
#include <windows.h>
#include <iostream>
#include <core/HIBIWindow.hpp>

class HTMLElement;

static const std::wstring DEFAULT_FONT = L"Arial";
static const std::wstring DEFAULT_CODE_FONT = L"Consolas";

class HIRender
{

public:
    HIRender() {};
    void DrawTreeRecursive(HDC hdc, const std::vector<std::shared_ptr<HTMLElement>> &elements,
                                 RECT rect, int &currentY);
                           
    static HFONT ElabFont(int cHeight, int cWeight, DWORD bItalic,
                    DWORD bUnderline, DWORD bStrikeOut,
                    DWORD iQuality, DWORD iPitchAndFamily, LPCWSTR pszFaceName);
    
private:

};