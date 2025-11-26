#include <core/HTMLParser.hpp>
#include <functional>
#include <unordered_set>

// Compute specificity: id=100, class=10, tag=1
int ComputeSpecificity(const std::wstring &selector)
{
    int spec = 0;
    if (selector.empty())
        return 0;
    if (selector[0] == L'#')
        spec += 100;
    else if (selector[0] == L'.')
        spec += 10;
    else
        spec += 1; // tag
    return spec;
}

void HtmlParser::ExtractStyles()
{
    // Match <style> blocks including newlines
    std::wregex styleRegex(LR"(<style[^>]*>[\s\S]*?<\/style>)", std::regex::icase);
    auto it = std::wsregex_iterator(html_.begin(), html_.end(), styleRegex);
    auto end = std::wsregex_iterator();

    for (; it != end; ++it)
    {
        std::wstring cssBlock = (*it).str();
        std::wregex inner(LR"(<style[^>]*>([\s\S]*?)<\/style>)", std::regex::icase);
        std::wsmatch m;
        if (std::regex_match(cssBlock, m, inner))
        {
            ParseCss(m[1].str()); // Parse inner CSS
        }
    }

    // Remove <style> block entirely from html_
    html_ = std::regex_replace(html_, styleRegex, L"");
}

void HtmlParser::ParseCss(const std::wstring &css)
{
    std::wregex ruleRe(LR"(([\w\.\#]+)\s*\{([^}]*)\})", std::regex::icase);
    auto it = std::wsregex_iterator(css.begin(), css.end(), ruleRe);
    auto end = std::wsregex_iterator();

    for (; it != end; ++it)
    {
        CssRule rule;
        rule.selector = (*it)[1].str();
        rule.style = StyleParser::ParseInline((*it)[2].str());
        rule.specificity = ComputeSpecificity(rule.selector);
        cssRules_.push_back(rule);
    }
}

Style HtmlParser::ApplyCssRules(const std::wstring &tag, const std::wstring &attr, const Style &base)
{
    Style s = base;
    std::wstring id;
    std::vector<std::wstring> classes;

    std::wsmatch m;
    std::wregex idRe(LR"(id\s*=\s*\"([^\"]*)\")", std::regex::icase);
    if (std::regex_search(attr, m, idRe))
        id = m[1].str();

    std::wregex classRe(LR"(class\s*=\s*\"([^\"]*)\")", std::regex::icase);
    if (std::regex_search(attr, m, classRe))
    {
        std::wstring cls = m[1].str();
        size_t start = 0;
        while (true)
        {
            size_t pos = cls.find(L' ', start);
            if (pos == std::wstring::npos)
            {
                classes.push_back(cls.substr(start));
                break;
            }
            classes.push_back(cls.substr(start, pos - start));
            start = pos + 1;
        }
    }

    // Apply rules sorted by specificity
    std::vector<std::pair<int, Style>> matchedStyles;
    for (auto &rule : cssRules_)
    {
        bool matched = false;
        if (rule.selector == tag)
            matched = true;
        else if (!id.empty() && rule.selector == L"#" + id)
            matched = true;
        else
        {
            for (auto &clsName : classes)
            {
                if (rule.selector == L"." + clsName)
                {
                    matched = true;
                    break;
                }
            }
        }
        if (matched)
            matchedStyles.emplace_back(rule.specificity, rule.style);
    }

    // Sort by specificity ascending
    std::sort(matchedStyles.begin(), matchedStyles.end(), [](auto &a, auto &b)
              { return a.first < b.first; });
    for (auto &[spec, style] : matchedStyles)
        s.Merge(style);

    // Merge inline style last
    std::wregex styleAttr(LR"(style\s*=\s*\"([^\"]*)\")", std::regex::icase);
    if (std::regex_search(attr, m, styleAttr))
    {
        Style inlineStyle = StyleParser::ParseInline(m[1].str());
        s.Merge(inlineStyle);
    }

    return s;
}

void ApplyParentStyles(Element &e, const Style &parentStyle)
{
    // Merge parent style first, preserving e.style specifics
    Style merged = parentStyle;
    merged.Merge(e.style);
    e.style = merged;

    for (auto &child : e.children)
        ApplyParentStyles(child, e.style);
}

std::vector<Element> HtmlParser::Parse()
{
    std::vector<Element> elements;
    std::wregex tagRegex(LR"(<(\/?)(\w+)([^>]*)>)", std::regex::icase);
    size_t lastPos = 0;
    std::wstring linkTarget;
    Style currentStyle;
    std::vector<Style> styleStack;

    // Track nested parents
    std::vector<std::vector<Element>*> elementStack;
    elementStack.push_back(&elements);

    // Layout variables
    int startX = 0;
    int startY = 0;
    int maxWidth = 800;
    int curX = startX;
    int curY = startY;
    int lineHeight = 0;

    auto it = std::wsregex_iterator(html_.begin(), html_.end(), tagRegex);
    auto end = std::wsregex_iterator();

    bool insideStyle = false;

    // Helper lambda for layout
    std::function<void(Element&)> layoutElement = [&](Element& e)
    {
        int fontSize = e.style.fontSize > 0 ? e.style.fontSize : 16;  // default font size
        int width = static_cast<int>(e.text.length() * fontSize * 0.6);
        int height = fontSize;

        // Block-level tags
        bool isBlock = e.tag == L"html" || e.tag == L"body" || e.tag == L"div" || e.tag == L"p" ||
                       e.tag == L"header" || e.tag == L"footer" || e.tag == L"section" || e.tag == L"article" ||
                       e.tag == L"nav" || e.tag == L"aside" || e.tag == L"main" || e.tag == L"ul" || e.tag == L"ol" ||
                       e.tag == L"table";

        if (isBlock)
        {
            curX = startX;
            curY += lineHeight;
            lineHeight = height;
            e.rect = { curX, curY, maxWidth, curY + height };
            curY += height;
        }
        else
        {
            if (curX + width > maxWidth)
            {
                curX = startX;
                curY += lineHeight;
                lineHeight = height;
            }
            e.rect = { curX, curY, curX + width, curY + height };
            curX += width;
            if (height > lineHeight)
                lineHeight = height;
        }

        // Layout children recursively
        int savedX = curX;
        int savedY = curY;
        int savedLine = lineHeight;

        curX = e.rect.left;
        curY = e.rect.top;
        lineHeight = 0;

        for (auto& child : e.children)
            layoutElement(child);

        curX = savedX;
        curY = savedY;
        lineHeight = savedLine;
    };

    for (; it != end; ++it)
    {
        size_t pos = it->position();
        std::wstring tag = (*it)[2].str();
        bool closing = (*it)[1].str() == L"/";
        std::wstring attr = (*it)[3].str();

        // Text between tags
        if (!insideStyle && pos > lastPos)
        {
            std::wstring text = html_.substr(lastPos, pos - lastPos);
            if (!text.empty())
            {
                Element e;
                e.text = text;
                e.style = currentStyle;
                e.startPos = lastPos;
                e.endPos = pos;
                if (!linkTarget.empty())
                {
                    e.href = linkTarget;
                    e.isLink = true;
                }
                elementStack.back()->push_back(e);
            }
        }

        if (tag == L"style")
        {
            insideStyle = !closing;
        }
        else if (!insideStyle)
        {
            if (!closing)
            {
                styleStack.push_back(currentStyle);
                currentStyle = ApplyCssRules(tag, attr, currentStyle);

                Element e;
                e.tag = tag;
                e.style = currentStyle;
                e.startPos = pos;
                e.endPos = pos + it->length();

                // Tag-specific handling
                if (tag == L"a")
                {
                    std::wregex hrefRe(LR"(href\s*=\s*\"([^\"]*)\")", std::regex::icase);
                    std::wsmatch m;
                    if (std::regex_search(attr, m, hrefRe))
                    {
                        e.href = m[1].str();
                        e.isLink = true;
                    }
                }
                else if (tag == L"img")
                {
                    std::wregex srcRe(LR"(src\s*=\s*\"([^\"]*)\")", std::regex::icase);
                    std::wsmatch m;
                    if (std::regex_search(attr, m, srcRe))
                    {
                        e.src = m[1].str();
                        e.isImage = true;
                    }
                }
                else if (tag == L"input")
                {
                    std::wregex valueRe(LR"(value\s*=\s*\"([^\"]*)\")", std::regex::icase);
                    std::wsmatch m;
                    if (std::regex_search(attr, m, valueRe))
                    {
                        e.inputValue = m[1].str();
                        e.isInput = true;
                    }
                }

                elementStack.back()->push_back(e);
                elementStack.push_back(&elementStack.back()->back().children);
            }
            else if (!styleStack.empty())
            {
                currentStyle = styleStack.back();
                styleStack.pop_back();
                if (!elementStack.empty())
                    elementStack.pop_back();
                if (tag == L"a")
                    linkTarget.clear();
            }
        }

        lastPos = pos + it->length();
    }

    // Remaining text after last tag
    if (!insideStyle && lastPos < html_.size())
    {
        std::wstring text = html_.substr(lastPos);
        if (!text.empty())
        {
            Element e;
            e.text = text;
            e.style = currentStyle;
            e.startPos = lastPos;
            e.endPos = html_.size();
            if (!linkTarget.empty())
            {
                e.href = linkTarget;
                e.isLink = true;
            }
            elementStack.back()->push_back(e);
        }
    }

    // Apply cascading styles and layout
    std::function<void(Element&, const Style&)> ApplyStylesAndLayout = [&](Element& e, const Style& parentStyle)
    {
        Style merged = parentStyle;
        merged.Merge(e.style);
        e.style = merged;

        layoutElement(e);

        for (auto& child : e.children)
            ApplyStylesAndLayout(child, e.style);
    };

    for (auto& e : elements)
        ApplyStylesAndLayout(e, Style());

    DebugLog->Log("Parsed " + std::to_string(elements.size()) + " elements with layout");
    return elements;
}
