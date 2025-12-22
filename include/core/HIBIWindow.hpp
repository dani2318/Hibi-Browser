#pragma once
#include <windows.h>
#include <string>

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
    private:
        WindowParams params;
        HINSTANCE hInst;
        HINSTANCE hPrevInst;
        PWSTR pCmdLine;
        int nCmdShow;
        WNDCLASSEXW wcex;
        HWND hwnd;
};