#include <windows.h>
#include <core/HIBIWindow.hpp>
#include <net/NetworkManager.hpp>
#include <fcntl.h> 
#include <io.h>

void SetupConsole(){
    AllocConsole();
    _setmode(_fileno(stdout), _O_U16TEXT);
    FILE* fp; 
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR pCmdLine, int nCmdShow) {

    #ifdef _DEBUG
        SetupConsole();
    #endif
    std::wcout << "[DEBUG] Console Attached." << std::endl;

    std::wcout << "[DEBUG] Attempting fetch..." << std::endl;
    std::string html = NetworkManager::Fetch(L"www.example.com", L"/");
    

    std::wcout << "[DEBUG] Bytes fetched: " << html.size() << std::endl;
    if (html.size() > 0) {
        std::wcout << "[DEBUG] " << html.c_str() << std::endl;
    } else {
        std::wcout << "[ERROR] Fetch returned empty string." << std::endl;
        exit(-1);
    }

    WindowParams params("HIBI Browser", 1280, 720);

    HIBIWindow* app = new HIBIWindow(
        params,
        hInst,
        hPrevInst,
        pCmdLine,
        nCmdShow
    );

    app->SetContent(html);

    return app->run();
}