#ifndef HIBIWINDOW_HPP
#define HIBIWINDOW_HPP

#include <windows.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "parser/HTMLParser.hpp"

struct WindowParams
{
    std::string title;
    int width;
    int height;
    WindowParams(std::string t, int w, int h) : title(t), width(w), height(h) {

    };
};

class HIBIWindow
{
private:
    WindowParams params;
    HINSTANCE hInst;
    HINSTANCE hPrevInst;
    PWSTR pCmdLine;
    int nCmdShow;
    WNDCLASSEXW wcex;
    HWND hwnd;
    
    std::wstring pageContent;
    
    HTMLParser* htmlParser;
    
    std::vector<HTMLElement> cachedElements;

    void DrawTreeRecursive(HDC hdc, const std::vector<HTMLElement> &elements, 
                          RECT rect, int &currentY);
    void PaintWindow(HIBIWindow *pWindow, const std::vector<HTMLElement> &elements);

public:
    explicit HIBIWindow(WindowParams p, HINSTANCE hInst, HINSTANCE hPrevInst, 
              PWSTR pCmdLine, int nCmdShow) ;
    
    ~HIBIWindow();
    
    HIBIWindow(const HIBIWindow&) = delete;
    HIBIWindow& operator=(const HIBIWindow&) = delete;
    
    void SetContent(const std::string &rawHtml);
    int run();
    
    friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif