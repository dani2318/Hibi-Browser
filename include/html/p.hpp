#pragma once
#include <windows.h>
#include <core/render/HIRender.hpp>
#include <html/HTMLElement.hpp>

class P : public HTMLElement 
{
public:
    P(std::wstring tag,
      std::map<std::wstring, std::wstring> attr,
      std::vector<HTMLElement> childs,
      std::wstring content = L"")
      : HTMLElement(tag, attr, childs, content) 
    {}

    // Override del metodo Font
    HFONT Font() override
    {
        return HIRender::ElabFont(-16, FW_NORMAL, FALSE, FALSE, FALSE,
                                  CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT.c_str());
    };
};