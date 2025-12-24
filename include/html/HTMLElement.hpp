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