#pragma once
#define NOMINMAX
#include <windows.h>
#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <unordered_map>
#include <core/DataType.hpp>
#include <core/StyleParser.hpp>
#include <core/Debug/Debug.hpp>



class HtmlParser
{
public:
    HtmlParser(const std::wstring &html, Logger *DebugLog)
        : html_(html), DebugLog(DebugLog)
    {
        ExtractStyles();
    }

    std::vector<Element> Parse();

private:
    std::wstring html_;
    Logger *DebugLog;
    std::vector<CssRule> cssRules_;

    void ExtractStyles();
    void ParseCss(const std::wstring &css);
    Style ApplyCssRules(const std::wstring &tag, const std::wstring &attr, const Style &base);
};
