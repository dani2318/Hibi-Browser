#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <regex>

class HTMLElement
{
public:
    HTMLElement(
        std::wstring tag,
        std::map<std::wstring, std::wstring> attr,
        std::vector<HTMLElement> childs,
        std::wstring content = L"") : tag(tag), attributes(attr), children(childs), content(content) {};

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

    std::vector<HTMLElement> children;

private:
    std::wstring tag;
    std::map<std::wstring, std::wstring> attributes;
    std::wstring content;
};