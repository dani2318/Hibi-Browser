#pragma once
#include <string>
#include <regex>
#include <core/DataType.hpp>
#include <unordered_map>
#include <sstream>

class StyleParser {
public:
    static Style ParseInline(const std::wstring &s) {
        Style st;

        // Regex to extract key:value pairs
        std::wregex kvRe(LR"(([\w-]+)\s*:\s*([^;]+);?)", std::regex::icase);
        std::wsmatch m;
        std::wstring input = s;
        auto begin = std::wsregex_iterator(input.begin(), input.end(), kvRe);
        auto end = std::wsregex_iterator();

        for (auto it = begin; it != end; ++it) {
            std::wstring key = it->str(1);
            std::wstring value = it->str(2);

            if (key == L"color") {
                st.color = ParseColor(value);
            } else if (key == L"background-color") {
                st.backgroundColor = ParseColor(value);
            } else if (key == L"font-weight") {
                st.bold = (value == L"bold");
            } else if (key == L"font-style") {
                st.italic = (value == L"italic");
            } else if (key == L"text-decoration") {
                st.underline = (value.find(L"underline") != std::wstring::npos);
                st.strikethrough = (value.find(L"line-through") != std::wstring::npos);
            } else if (key == L"font-size") {
                st.fontSize = ParseFontSize(value);
            }
            // You can add more CSS properties here if needed
        }

        return st;
    }

private:
    static COLORREF ParseColor(const std::wstring &value) {
        // Named colors
        if (value == L"red") return RGB(255, 0, 0);
        if (value == L"green") return RGB(0, 128, 0);
        if (value == L"blue") return RGB(0, 0, 255);
        if (value == L"black") return RGB(0, 0, 0);
        if (value == L"white") return RGB(255, 255, 255);

        // Hex color #RRGGBB
        std::wregex hexRe(LR"#(#([0-9a-fA-F]{6}))#");
        std::wsmatch m;
        if (std::regex_match(value, m, hexRe)) {
            int r = std::stoi(m[1].str().substr(0, 2), nullptr, 16);
            int g = std::stoi(m[1].str().substr(2, 2), nullptr, 16);
            int b = std::stoi(m[1].str().substr(4, 2), nullptr, 16);
            return RGB(r, g, b);
        }

        return RGB(0, 0, 0); // default black
    }

    static int ParseFontSize(const std::wstring &value) {
        std::wregex sizeRe(LR"((\d+)px)");
        std::wsmatch m;
        if (std::regex_match(value, m, sizeRe)) {
            return std::stoi(m[1].str());
        }
        return 12; // default font size
    }
};
