#pragma once 
#include <string>
#include <map>
#include <vector>
#include <iostream>


struct HTMLElement {
    std::string tag;
    std::map<std::string, std::string> attributes;
    std::vector <HTMLElement> children;
};

class HTMLParser {
    public:
        HTMLParser(const std::string &html) : html_(html), index_(0) {};
    private:
        const std::string& html_;
        const int index_;
};