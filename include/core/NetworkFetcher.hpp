#pragma once

#define NOMINMAX
#include <windows.h>
#include <wininet.h>
#include <core/Utils.hpp>
#include <thread>
#include <functional>

class NetworkFetcher {
public:
    
    static void Fetch(const std::wstring &url, std::function<void(const std::wstring& )> callback) {
        std::thread([url, callback]() {
            Logger* DebugLog = new Logger();

            HINTERNET hInternet = InternetOpenW(L"HibiBrowser/1.0", INTERNET_OPEN_TYPE_DIRECT, nullptr, nullptr, 0);
            if (!hInternet) { DebugLog->Log("InternetOpenW failed"); return; }

            URL_COMPONENTSW urlComp{};
            urlComp.dwStructSize = sizeof(urlComp);
            wchar_t hostName[256], urlPath[2048];
            urlComp.lpszHostName = hostName; urlComp.dwHostNameLength = _countof(hostName);
            urlComp.lpszUrlPath = urlPath; urlComp.dwUrlPathLength = _countof(urlPath);

            if (!InternetCrackUrlW(url.c_str(), 0, 0, &urlComp)) {
                DebugLog->Log("InternetCrackUrlW failed"); InternetCloseHandle(hInternet); return;
            }

            DWORD flags = INTERNET_FLAG_RELOAD;
            if (urlComp.nScheme == INTERNET_SCHEME_HTTPS)
                flags |= INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;

            HINTERNET hConnect = InternetConnectW(hInternet, hostName, urlComp.nPort, nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0);
            if (!hConnect) { DebugLog->Log("InternetConnectW failed"); InternetCloseHandle(hInternet); return; }

            const wchar_t* acceptTypes[] = { L"*/*", nullptr };
            HINTERNET hRequest = HttpOpenRequestW(hConnect, L"GET", urlPath, nullptr, nullptr, acceptTypes, flags, 0);
            if (!hRequest) { DebugLog->Log("HttpOpenRequestW failed"); InternetCloseHandle(hConnect); InternetCloseHandle(hInternet); return; }

            if (!HttpSendRequestW(hRequest, nullptr, 0, nullptr, 0)) {
                DebugLog->Log("HttpSendRequestW failed"); InternetCloseHandle(hRequest); InternetCloseHandle(hConnect); InternetCloseHandle(hInternet); return;
            }

            char buffer[4096]; DWORD bytesRead = 0; std::string html;
            while (InternetReadFile(hRequest, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0)
                html.append(buffer, bytesRead);

            InternetCloseHandle(hRequest); InternetCloseHandle(hConnect); InternetCloseHandle(hInternet);

            int size_needed = MultiByteToWideChar(CP_UTF8, 0, html.c_str(), (int)html.size(), nullptr, 0);
            std::wstring whtml(size_needed, L'\0');
            MultiByteToWideChar(CP_UTF8, 0, html.c_str(), (int)html.size(), &whtml[0], size_needed);

            callback(whtml);
            DebugLog->Log("Finished fetching URL: " + ToUtf8(url));
            delete DebugLog;
        }).detach();
    }
};