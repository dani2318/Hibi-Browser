#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <regex>
#include <core/render/HIRender.hpp>

class HTMLElement
{
public:
    HTMLElement(
        std::wstring tag,
        std::map<std::wstring, std::wstring> attr,
        std::vector<std::shared_ptr<HTMLElement>> childs,
        std::wstring content = L"") : tag(tag), attributes(attr), children(childs), content(content) {};

    HTMLElement(const std::shared_ptr<HTMLElement>& other)
        : tag(other->tag), attributes(other->attributes), children(other->children), content(other->content) {}

    std::wstring GetTag() const { return tag; };
    HTMLElement *GetChildren(int index)
    {
        if (index >= 0 && index < children.size())
        {
            return children[index].get();
        }
        return nullptr;
    }
    std::wstring GetContent() const { return content; };
    virtual ~HTMLElement() = default;

    std::vector<std::shared_ptr<HTMLElement>> children;
private:
    std::wstring tag;
    std::map<std::wstring, std::wstring> attributes;
    std::wstring content;
};

class HTMLTextElement : public HTMLElement
{
public:
    HTMLTextElement(
        std::wstring tag,
        std::map<std::wstring, std::wstring> attr,
        std::vector<std::shared_ptr<HTMLElement>> childs,
        std::wstring content = L"") : HTMLElement(tag, attr, childs, content){};

    std::wstring GetTag() const { return tag; };
    HTMLElement *GetChildren(int index)
    {
        if (index >= 0 && index < children.size())
        {
            return &children[index];
        }
        return nullptr;
    }
    std::wstring GetContent() const { return content; };

    virtual ~HTMLTextElement() = default;

    std::vector<HTMLElement> children;
    virtual HFONT Font() const {
        return HIRender::ElabFont(-16, FW_NORMAL, FALSE, FALSE, FALSE,
                                   CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT.c_str());
    };

private:
    std::wstring tag;
    std::map<std::wstring, std::wstring> attributes;
    std::wstring content;
};


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
        // Standard paragraph font
        return HIRender::ElabFont(-16, FW_NORMAL, FALSE, FALSE, FALSE,
                                   CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                                   DEFAULT_FONT.c_str());
    }
    virtual ~HTMLParagraph() = default;

};

class HTMLH1 : public HTMLTextElement
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

class HTMLDiv : public HTMLElement
{
public:
    HTMLDiv(
        std::wstring tag,
        std::map<std::wstring, std::wstring> attr,
        std::vector<std::shared_ptr<HTMLElement>> childs,
        std::wstring content = L"")
        : HTMLElement(tag=L"div", attr, childs, content) {}

    virtual ~HTMLDiv() = default;
};