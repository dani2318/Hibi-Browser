#pragma once
#include <string>
#include <map>
#include <vector>

#include <html/HTMLTextElement.hpp> 
#include <fonts/IFontProvider.hpp>

class HTMLH1 : public HTMLTextElement, public IFontProvider
{
public:
    HTMLH1(
        std::wstring tag,
        std::map<std::wstring, std::wstring> attr,
        std::vector<std::shared_ptr<HTMLElement>> childs,
        std::wstring content = L"")
        : HTMLTextElement(tag=L"p", attr, childs, content) {}
    
    HFONT Font() const override
    {
        // Standard paragraph font
        return HIRender::ElabFont(-32, FW_BOLD, FALSE, FALSE, FALSE,
                                 CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT.c_str());
    }
    virtual ~HTMLH1() = default;

};

class HTMLH2 : public HTMLTextElement
{
public:
    HTMLH2(
        std::wstring tag,
        std::map<std::wstring, std::wstring> attr,
        std::vector<std::shared_ptr<HTMLElement>> childs,
        std::wstring content = L"")
        : HTMLTextElement(tag=L"p", attr, childs, content) {}
    
        
    HFONT Font() const override
    {
        // Standard paragraph font
        return HIRender::ElabFont(-24, FW_BOLD, FALSE, FALSE, FALSE,
                                 CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT.c_str());
    }
    virtual ~HTMLH2() = default;

};

class HTMLH3 : public HTMLTextElement
{
public:
    HTMLH3(
        std::wstring tag,
        std::map<std::wstring, std::wstring> attr,
        std::vector<std::shared_ptr<HTMLElement>> childs,
        std::wstring content = L"")
        : HTMLTextElement(tag=L"p", attr, childs, content) {}
    
    HFONT Font() const override
    {
        // Standard paragraph font
        return HIRender::ElabFont(-20, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                                 CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT.c_str());
    }
    virtual ~HTMLH3() = default;
};