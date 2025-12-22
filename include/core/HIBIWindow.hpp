#pragma once
#include <windows.h>
#include <string>
#include <tchar.h>
#include <regex>
#include <vector>
#include <iostream>
#include <map>
#include <stack>
#include <algorithm>

struct TaggedContent
{
    std::wstring tag;
    std::wstring text;
    std::map<std::wstring, std::wstring> attributes;
    std::vector<TaggedContent> children;
};

struct WindowParams{
    int width;
    int height;
    std::string title;
};

class HIBIWindow{
    public:
        HIBIWindow(WindowParams p, HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR pCmdLine, int nCmdShow) ;
        std::wstring pageContent;
        void SetContent(const std::string& rawHtml);    
        int run();
        void PaintWindow(HIBIWindow* pWindow, const std::vector<TaggedContent>& elements);
    private:
        WindowParams params;
        HINSTANCE hInst;
        HINSTANCE hPrevInst;
        PWSTR pCmdLine;
        int nCmdShow;
        WNDCLASSEXW wcex;
        HWND hwnd;

        void DrawTreeRecursive(HDC hdc, const std::vector<TaggedContent>& elements, RECT rect, int& currentY);
};