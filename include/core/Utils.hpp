#pragma once

#define NOMINMAX
#include <windows.h>
#include <string>

#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))



std::string ToUtf8(const std::wstring &s) {
    if (s.empty()) return {};
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0, nullptr, nullptr);
    std::string out(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, s.c_str(), (int)s.size(), &out[0], size_needed, nullptr, nullptr);
    return out;
}
