#include <windows.h>
#include <core/HIBIWindow.hpp>
#include <net/NetworkManager.hpp>
// #include <iostream> // Not needed if using OutputDebugString

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR pCmdLine, int nCmdShow) {
    
    AllocConsole();
    FILE* fp; 
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);

    std::cout << "[DEBUG] Console Attached." << std::endl;

    std::cout << "[DEBUG] Attempting fetch..." << std::endl;
    std::string html = NetworkManager::Fetch(L"www.example.com", L"/");
    

    std::cout << "[DEBUG] Bytes fetched: " << html.size() << std::endl;
    if (html.size() > 0) {
        std::cout << "[DEBUG] " << html.c_str() << std::endl;
    } else {
        std::cout << "[ERROR] Fetch returned empty string." << std::endl;
    }

    // 4. Create Window (Stack allocation is safer here)
    HIBIWindow app(
        { 1280, 720, "HIBI Browser" },
        hInst,
        hPrevInst,
        pCmdLine,
        nCmdShow
    );

    app.SetContent(html);

    return app.run();
}