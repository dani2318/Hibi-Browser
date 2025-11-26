#include <core/BrowserWindow.hpp>
#include <core/Debug/Debug.hpp>

#pragma comment(lib, "wininet.lib")

Logger* DebugLog = new Logger();

// ------------------ Entry Point ------------------
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PWSTR pCmdLine, int nCmdShow) {
    BrowserWindow app(hInst, DebugLog);
    app.Run();
    return 0;
}