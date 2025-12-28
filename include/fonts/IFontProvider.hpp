#pragma once
#include <windows.h>

class IFontProvider {
public:
    virtual ~IFontProvider() = default;
    virtual HFONT Font() const = 0;
};