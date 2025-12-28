#pragma once
#include <windows.h>

class IFontProvider {
public:
    virtual ~IFontProvider() = default;
    virtual HFONT Font() = 0; // Pure virtual
};