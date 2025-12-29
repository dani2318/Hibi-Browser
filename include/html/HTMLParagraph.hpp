#pragma once
#include <string>
#include <map>
#include <vector>


#include <html/HTMLTextElement.hpp> 

class HTMLParagraph : public HTMLTextElement
{
public:
    HTMLParagraph(
        std::wstring tag,
        std::map<std::wstring, std::wstring> attr,
        std::vector<std::shared_ptr<HTMLElement>> childs,
        std::wstring content = L"")
        : HTMLTextElement(tag=L"p", attr, childs, content) {}
    
    HFONT Font() const override
    {
        return HIRender::ElabFont(-16, FW_NORMAL, FALSE, FALSE, FALSE,
                                   CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                   DEFAULT_FONT.c_str());
    }
    virtual ~HTMLParagraph() = default;

};