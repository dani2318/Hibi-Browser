
#include <core/parser/HTMLParser.hpp>
#include <algorithm>
#include <cctype>
#include <html/HTMLElement.hpp>
#include <html/HTMLParagraph.hpp>
#include <html/HTMLHeaders.hpp>
#include <sstream>

std::string wstring_to_string(const std::wstring& wstr)
{
    std::string str(wstr.begin(), wstr.end());
    return str;
}

std::wstring string_to_wstring(const std::string& str)
{
    std::wstring wstr(str.begin(), str.end());
    return wstr;
}

std::map<std::wstring, std::wstring> HTMLParser::parseAttributes(const std::wstring &attrStr)
{
    std::map<std::wstring, std::wstring> attrs;

    if (attrStr.empty())
    {
        return attrs;
    }

    try
    {
        std::wregex attrRegex(L"(\\w+)\\s*=\\s*(?:\"([^\"]*)\"|'([^']*)'|(\\S+))");
        auto attrs_begin = std::wsregex_iterator(attrStr.begin(), attrStr.end(), attrRegex);
        auto attrs_end = std::wsregex_iterator();

        for (auto i = attrs_begin; i != attrs_end; ++i)
        {
            std::wsmatch match = *i;
            std::wstring attrName = match[1].str();
            std::wstring attrValue;

            if (match[2].matched)
            {
                attrValue = match[2].str();
            }
            else if (match[3].matched)
            {
                attrValue = match[3].str();
            }
            else if (match[4].matched)
            {
                attrValue = match[4].str();
            }

            attrs[attrName] = attrValue;
        }
    }
    catch (const std::exception &e)
    {
        std::wcerr << L"Error parsing attributes: " << e.what() << std::endl;
    }

    return attrs;
}

void HTMLParser::parseStyleTag(const std::wstring& cssContent, 
                                std::map<std::wstring, CSSProperties>& styleRules)
{
    if (cssContent.empty())
        return;
    
    try
    {
        std::string cssStr = wstring_to_string(cssContent);
        
        cssStr = std::regex_replace(cssStr, std::regex("/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/"), "");
        
        std::regex ruleRegex("([^{]+)\\{([^}]+)\\}");
        auto rules_begin = std::sregex_iterator(cssStr.begin(), cssStr.end(), ruleRegex);
        auto rules_end = std::sregex_iterator();
        
        for (auto it = rules_begin; it != rules_end; ++it)
        {
            std::smatch match = *it;
            std::string selector = match[1].str();
            std::string declarations = match[2].str();
            
            selector.erase(0, selector.find_first_not_of(" \t\n\r"));
            selector.erase(selector.find_last_not_of(" \t\n\r") + 1);
            
            CSSProperties props;
            props.fromCSSString(declarations);
            
            styleRules[string_to_wstring(selector)] = props;
        }
    }
    catch (const std::exception& e)
    {
        std::wcerr << L"Error parsing style tag: " << e.what() << std::endl;
    }
}

void HTMLParser::applyCSSRules(std::shared_ptr<HTMLElement> element,
                                const std::map<std::wstring, CSSProperties>& styleRules)
{
    if (!element)
        return;
    
    std::wstring tag = element->GetTag();
    auto tagRule = styleRules.find(tag);
    if (tagRule != styleRules.end())
    {
        element->GetCSSProperties().merge(tagRule->second);
    }
    
    auto& attrs = element->GetAttributes();
    auto classAttr = attrs.find(L"class");
    if (classAttr != attrs.end())
    {
        std::wstring className = L"." + classAttr->second;
        auto classRule = styleRules.find(className);
        if (classRule != styleRules.end())
        {
            element->GetCSSProperties().merge(classRule->second);
        }
    }
    
    auto idAttr = attrs.find(L"id");
    if (idAttr != attrs.end())
    {
        std::wstring idName = L"#" + idAttr->second;
        auto idRule = styleRules.find(idName);
        if (idRule != styleRules.end())
        {
            element->GetCSSProperties().merge(idRule->second);
        }
    }
    
    for (auto& child : element->children)
    {
        applyCSSRules(child, styleRules);
    }
}

static size_t readUntil(const std::wstring& s, size_t i, wchar_t stop) {
    while (i < s.size() && s[i] != stop) ++i;
    return i;
}

static std::wstring collapseWhitespace(const std::wstring& text) {
    std::wstring out;
    bool space = false;

    for (wchar_t c : text) {
        if (iswspace(c)) {
            if (!space) out += L' ';
            space = true;
        } else {
            out += c;
            space = false;
        }
    }
    return out;
}

static std::map<std::wstring, std::wstring>
parseAttributesSafe(const std::wstring& s)
{
    std::map<std::wstring, std::wstring> attrs;
    size_t i = 0;

    while (i < s.size()) {
        while (i < s.size() && iswspace(s[i])) ++i;
        if (i >= s.size()) break;

        size_t keyStart = i;
        while (i < s.size() &&
               !iswspace(s[i]) &&
               s[i] != L'=' &&
               s[i] != L'>' &&
               s[i] != L'/')
            ++i;

        std::wstring key = s.substr(keyStart, i - keyStart);
        std::transform(key.begin(), key.end(), key.begin(), ::towlower);

        while (i < s.size() && iswspace(s[i])) ++i;

        if (i >= s.size() || s[i] != L'=') {
            attrs[key] = L"true";
            continue;
        }

        ++i;
        while (i < s.size() && iswspace(s[i])) ++i;

        wchar_t quote = 0;
        if (i < s.size() && (s[i] == L'"' || s[i] == L'\'')) {
            quote = s[i++];
        }

        size_t valStart = i;
        while (i < s.size() &&
              ((quote && s[i] != quote) ||
               (!quote && !iswspace(s[i]) && s[i] != L'>')))
            ++i;

        std::wstring value = s.substr(valStart, i - valStart);
        attrs[key] = value;

        if (quote && i < s.size()) ++i;
    }

    return attrs;
}


std::vector<std::shared_ptr<HTMLElement>>
HTMLParser::parseHtmlToTree(const std::wstring& html)
{
    HTMLElement root(L"root", {}, {});
    std::stack<HTMLElement*> stack;
    stack.push(&root);

    std::map<std::wstring, CSSProperties> styleRules;
    bool insideStyle = false;
    std::wstring styleBuffer;

    const std::vector<std::wstring> voidTags = {
        L"br", L"img", L"hr", L"meta", L"link", L"input"
    };

    size_t i = 0;
    size_t nodeCount = 0;

    while (i < html.size()) {

        if (nodeCount > 100000) break;

        if (html[i] == L'<') {

            if (i + 1 < html.size() && html[i + 1] == L'/') {
                size_t nameStart = i + 2;
                size_t nameEnd = readUntil(html, nameStart, L'>');

                std::wstring tag = html.substr(nameStart, nameEnd - nameStart);
                std::transform(tag.begin(), tag.end(), tag.begin(), ::towlower);

                if (tag == L"style") {
                    insideStyle = false;
                    parseStyleTag(styleBuffer, styleRules);
                    styleBuffer.clear();
                }

                if (stack.size() > 1 && stack.top()->GetTag() == tag)
                    stack.pop();

                i = nameEnd + 1;
                continue;
            }

            size_t nameStart = i + 1;
            size_t nameEnd = nameStart;

            while (nameEnd < html.size() &&
                   !iswspace(html[nameEnd]) &&
                   html[nameEnd] != L'>' &&
                   html[nameEnd] != L'/')
                ++nameEnd;

            size_t tagEnd = readUntil(html, nameEnd, L'>');

            if (tagEnd - i > 2048) {
                i = tagEnd + 1;
                continue;
            }

            std::wstring tag = html.substr(nameStart, nameEnd - nameStart);
            std::transform(tag.begin(), tag.end(), tag.begin(), ::towlower);

            bool selfClosing = (tagEnd > 0 && html[tagEnd - 1] == L'/');

            std::wstring attrStr = html.substr(nameEnd, tagEnd - nameEnd);
            auto attrs = parseAttributesSafe(attrStr);

            if (tag == L"style") {
                insideStyle = true;
                styleBuffer.clear();
                i = tagEnd + 1;
                continue;
            }

            auto node = std::make_shared<HTMLElement>(
                tag, attrs, std::vector<std::shared_ptr<HTMLElement>>{}
            );

            stack.top()->children.push_back(node);
            ++nodeCount;

            bool isVoid = std::find(voidTags.begin(), voidTags.end(), tag) != voidTags.end();

            if (!selfClosing && !isVoid) {
                stack.push(node.get());
                if (stack.size() > 128) stack.pop();
            }

            i = tagEnd + 1;
        }
        else {
            size_t textEnd = readUntil(html, i, L'<');
            std::wstring text = html.substr(i, textEnd - i);

            if (insideStyle) {
                if (styleBuffer.size() < 64 * 1024)
                    styleBuffer += text;
            }
            else {
                text = collapseWhitespace(text);
                if (!text.empty() && !stack.empty()) {

                    auto& children = stack.top()->children;

                    if (!children.empty() &&
                        children.back()->GetTag() == L"text") {
                        children.back()->AppendContent(text);
                    }
                    else {
                        auto textNode = std::make_shared<HTMLElement>(
                            L"text", std::map<std::wstring,std::wstring>{},
                            std::vector<std::shared_ptr<HTMLElement>>{}, text
                        );
                        children.push_back(textNode);
                        ++nodeCount;
                    }
                }
            }

            i = textEnd;
        }
    }

    if (insideStyle) {
        parseStyleTag(styleBuffer, styleRules);
    }

    for (auto& child : root.children)
        applyCSSRules(child, styleRules);

    return root.children;
}

