#pragma once 
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <regex>
#include <map>
#include <html/HTMLElement.hpp>
#include <stack>

class HTMLParser {
    public:
        HTMLParser(std::wstring* html) : html_(html) {};
        std::vector<HTMLElement> parseHtmlToTree(const std::wstring &html);
    private:
        std::wstring* html_;
        std::map<std::wstring, std::wstring> parseAttributes(const std::wstring &attrStr);
};