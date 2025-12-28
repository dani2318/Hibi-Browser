#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <regex>
#include <memory>
#include <css/CSSProperties.hpp>

class HTMLElement
{
public:
    HTMLElement(
        std::wstring tag,
        std::map<std::wstring, std::wstring> attr,
        std::vector<std::shared_ptr<HTMLElement>> childs,
        std::wstring content = L"") 
        : tag(tag), attributes(attr), children(childs), content(content) 
    {
        parseInlineStyles();
    }
    
    HTMLElement(const std::shared_ptr<HTMLElement>& other)
        : tag(other->tag), 
          attributes(other->attributes), 
          children(other->children), 
          content(other->content),
          cssProperties(other->cssProperties) {}
    
    std::wstring GetTag() const { return tag; }
    
    HTMLElement *GetChildren(int index)
    {
        if (index >= 0 && index < children.size())
        {
            return children[index].get();
        }
        return nullptr;
    }
    
    const std::wstring& GetContent() const {
        return content;
    }
    void SetContent(const std::wstring& newContent) {
        content = newContent;
    }
    void AppendContent(const std::wstring& more) {
        content += more;
    }

    CSSProperties& GetCSSProperties() { return cssProperties; }
    const CSSProperties& GetCSSProperties() const { return cssProperties; }
    
    void SetCSSProperty(const std::string& property, const std::string& value)
    {
        cssProperties.setProperty(property, value);
    }
    
    std::string GetCSSProperty(const std::string& property) const
    {
        return cssProperties.getProperty(property);
    }
    
    std::map<std::wstring, std::wstring>& GetAttributes() { return attributes; }
    const std::map<std::wstring, std::wstring>& GetAttributes() const { return attributes; }
    
    virtual ~HTMLElement() = default;
    
    std::vector<std::shared_ptr<HTMLElement>> children;

protected:
    void parseInlineStyles()
    {
        auto it = attributes.find(L"style");
        if (it != attributes.end())
        {
            std::wstring styleStr = it->second;
            std::string styleStrNarrow(styleStr.begin(), styleStr.end());
            cssProperties.fromCSSString(styleStrNarrow);
        }
    }

private:
    std::wstring tag;
    std::map<std::wstring, std::wstring> attributes;
    std::wstring content;
    CSSProperties cssProperties;
};

class HTMLDiv : public HTMLElement
{
public:
    HTMLDiv(
        std::wstring tag,
        std::map<std::wstring, std::wstring> attr,
        std::vector<std::shared_ptr<HTMLElement>> childs,
        std::wstring content = L"")
        : HTMLElement(L"div", attr, childs, content) {}
    virtual ~HTMLDiv() = default;
};