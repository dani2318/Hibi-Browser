#pragma once
#include <windows.h>
#include <iostream>
#include <winhttp.h>
#include <string>

#pragma comment(lib, "winhttp.lib")

class NetworkManager
{
public:
    static std::string Fetch(LPCWSTR domain, LPCWSTR path)
    {
        std::string responseData;
        HINTERNET hSession = WinHttpOpen(L"HIBIBrowser/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, NULL, NULL, 0);
        HINTERNET hConnect = WinHttpConnect(hSession, domain, INTERNET_DEFAULT_HTTPS_PORT, 0);
        HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", path, NULL, NULL, NULL, WINHTTP_FLAG_SECURE);

        if (WinHttpSendRequest(hRequest, NULL, 0, NULL, 0, 0, 0) && WinHttpReceiveResponse(hRequest, NULL))
        {
            DWORD dwSize = 0;
            do
            {
                WinHttpQueryDataAvailable(hRequest, &dwSize);
                char *buffer = new char[dwSize + 1];
                DWORD dwDownloaded = 0;
                WinHttpReadData(hRequest, (LPVOID)buffer, dwSize, &dwDownloaded);
                buffer[dwDownloaded] = '\0';
                responseData += buffer;
                delete[] buffer;
            } while (dwSize > 0);
        }

        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);

        std::cout << responseData << std::endl;

        return responseData;
    };
};