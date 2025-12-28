#pragma once
#include <string>
#include <map>
#include <vector>
#include <stack>
#include <regex>
#include <memory>
#include <css/CSSProperties.hpp>

class HTMLElement;

class HTMLParser
{
public:
    HTMLParser(std::wstring* html) : html_(html) {};
    static std::map<std::wstring, std::wstring> parseAttributes(const std::wstring &attrStr);
    static std::vector<std::shared_ptr<HTMLElement>> parseHtmlToTree(const std::wstring &html);
    
private:
    std::wstring* html_;

    static void parseStyleTag(const std::wstring& cssContent, 
                              std::map<std::wstring, CSSProperties>& styleRules);
    static void applyCSSRules(std::shared_ptr<HTMLElement> element,
                              const std::map<std::wstring, CSSProperties>& styleRules);
};
