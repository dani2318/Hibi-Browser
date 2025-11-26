#pragma once
#define NOMINMAX
#include <windows.h>
#include <string>

// ------------------ Style ------------------

struct Style
{
    // Text styling
    bool bold = false;
    bool italic = false;
    bool underline = false;
    bool strikethrough = false;
    int fontSize = 16;            // Default font size
    std::wstring fontFamily = L"Segoe UI";

    // Colors
    uint32_t color = 0xFF000000;  // Default black, ARGB
    uint32_t backgroundColor = 0x00000000; // Transparent

    // Box model
    int marginTop = 0;
    int marginBottom = 0;
    int marginLeft = 0;
    int marginRight = 0;

    int paddingTop = 0;
    int paddingBottom = 0;
    int paddingLeft = 0;
    int paddingRight = 0;

    // Methods
    void Merge(const Style &other)
    {
        if (other.bold) bold = true;
        if (other.italic) italic = true;
        if (other.underline) underline = true;
        if (other.fontSize != 0) fontSize = other.fontSize;
        if (!other.fontFamily.empty()) fontFamily = other.fontFamily;
        if (other.color != 0) color = other.color;
        if (other.backgroundColor != 0) backgroundColor = other.backgroundColor;

        // Merge margins/padding only if non-zero
        if (other.marginTop != 0) marginTop = other.marginTop;
        if (other.marginBottom != 0) marginBottom = other.marginBottom;
        if (other.marginLeft != 0) marginLeft = other.marginLeft;
        if (other.marginRight != 0) marginRight = other.marginRight;

        if (other.paddingTop != 0) paddingTop = other.paddingTop;
        if (other.paddingBottom != 0) paddingBottom = other.paddingBottom;
        if (other.paddingLeft != 0) paddingLeft = other.paddingLeft;
        if (other.paddingRight != 0) paddingRight = other.paddingRight;
    }

    bool IsEmpty() const
    {
        return !bold && !italic && !underline &&
               fontSize == 16 && fontFamily.empty() &&
               color == 0xFF000000 && backgroundColor == 0x00000000 &&
               marginTop == 0 && marginBottom == 0 &&
               marginLeft == 0 && marginRight == 0 &&
               paddingTop == 0 && paddingBottom == 0 &&
               paddingLeft == 0 && paddingRight == 0;
    }
};


// ------------------ Page Element ------------------
struct Element {
    RECT rect{};                 // Position/size, can be calculated later
    std::wstring text;           // Text content
    std::wstring href;           // Link URL if <a>
    std::wstring src;            // Image source if <img>
    std::wstring inputValue;     // Input value if <input>
    std::wstring tag;            // Tag name for generic elements
    Style style;                 // Computed style for this element

    bool isLink = false;         // True if <a>
    bool isImage = false;        // True if <img>
    bool isInput = false;        // True if <input>
    bool isGeneric = false;      // True for other HTML nodes without special handling

    size_t startPos = 0;         // Start position in original HTML
    size_t endPos = 0;           // End position in original HTML
    std::vector<Element> children; // Children elements
};




struct CssRule
{
    std::wstring selector; // tag, .class, #id
    Style style;
    int specificity; // computed when parsing
};