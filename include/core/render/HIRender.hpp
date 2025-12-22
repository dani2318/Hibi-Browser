#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <core/HIBIWindow.hpp>
#include <core/parser/HTMLParser.hpp>

class HIRender{

    public:
        HIRender(std::wstring html, HTMLParser* htmlParser) : html_(html), htmlParser(htmlParser){};

        static bool Render(PAINTSTRUCT ps, HDC hdc, HIBIWindow *pWindow, HWND hwnd){
            if(pWindow == nullptr || pWindow->pageContent.empty()) return false;

            RECT rect;
            GetClientRect(hwnd, &rect);

            FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

            int yOffset = rect.top + 20;


            EndPaint(hwnd, &ps);
            return true;
        }

    private:
        std::wstring html_;
        HTMLParser* htmlParser;

};