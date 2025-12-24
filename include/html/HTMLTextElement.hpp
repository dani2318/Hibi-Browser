#pragma once
#include <string>
#include <map>
#include <vector>

#include <html/HTMLElement.hpp>

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
