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
        std::string tag,
        std::map<std::string, std::string> attr,
        std::vector<HTMLElement> childs,
        std::string content = ""
    ) : tag(tag), attributes(attr), children(childs), content(content){
        
        std::string pattern = "<" + tag + R"([^>]*>([\s\S]*?)<\/)" + tag + ">";
        regex = std::regex(pattern, std::regex::icase);

    };

    std::regex GetRegex() const { return regex; };
    std::string GetTag() const { return tag; };
    HTMLElement* GetChildren(int index) { 
        if (index >= 0 && index < children.size()) {
            return &children[index]; 
        }
        return nullptr; // Safety check
    }
    std::string GetContent() const { return content; };

private:
    std::regex regex;
    std::string tag;
    std::map<std::string, std::string> attributes;
    std::vector<HTMLElement> children;
    std::string content;
};