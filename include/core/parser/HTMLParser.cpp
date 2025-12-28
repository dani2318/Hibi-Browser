#include <core/parser/HTMLParser.hpp>
#include <algorithm>
#include <cctype>
#include <html/HTMLElement.hpp>
#include <html/HTMLParagraph.hpp>
#include <html/HTMLHeaders.hpp>

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

std::vector<std::shared_ptr<HTMLElement>> HTMLParser::parseHtmlToTree(const std::wstring &html)
{
    if (html.empty())
    {
        return std::vector<std::shared_ptr<HTMLElement>>();
    }

    try
    {
        HTMLElement root(L"root", std::map<std::wstring, std::wstring>(),
                         std::vector<std::shared_ptr<HTMLElement>>());

        std::stack<HTMLElement *> nodeStack;
        nodeStack.push(&root);

        std::wregex tokenRegex(L"|<(/?)([a-zA-Z!][\\w:-]*)([^>]*)>|([^<]+)");
        auto begin = std::wsregex_iterator(html.begin(), html.end(), tokenRegex);
        auto end = std::wsregex_iterator();

        std::vector<std::wstring> voidTags = {
            L"area", L"base", L"br", L"col", L"embed", L"hr", L"img",
            L"input", L"link", L"meta", L"param", L"source", L"track", L"wbr"};

        int tagCount = 0;
        const int MAX_TAGS = 10000;

        for (auto it = begin; it != end && tagCount < MAX_TAGS; ++it, ++tagCount)
        {
            std::wsmatch match = *it;
            std::wstring fullMatch = match[0].str();

            if (fullMatch.find(L"<!--") == 0)
            {
                continue;
            }

            if (match[2].matched)
            {
                std::wstring closingSlash = match[1].str();
                std::wstring tagName = match[2].str();
                std::wstring attrStr = match[3].str();

                bool isClosingTag = !closingSlash.empty();
                bool isSelfClosing = (attrStr.find(L"/") != std::wstring::npos);

                std::transform(tagName.begin(), tagName.end(), tagName.begin(), ::towlower);

                if (!isClosingTag)
                {
                    std::map<std::wstring, std::wstring> attrs = parseAttributes(attrStr);

                    if (tagName == L"p")
                    {
                        HTMLParagraph pNode(tagName, attrs, std::vector<std::shared_ptr<HTMLElement>>());
                        std::wcout << "Using HTMLParagraph instead of HTMLElement"<< std::endl;

                        if (!nodeStack.empty())
                        {
                            nodeStack.top()->children.push_back(std::make_shared<HTMLParagraph>(pNode));

                            bool isVoid = std::find(voidTags.begin(), voidTags.end(), tagName) != voidTags.end();

                            if (!isSelfClosing && !isVoid && nodeStack.size() < 100)
                                nodeStack.push(nodeStack.top()->children.back().get());
                        }

                    }
                    else if (tagName == L"div"){
                        HTMLDiv pNode(tagName, attrs, std::vector<std::shared_ptr<HTMLElement>>());
                        std::wcout << "Using HTMLDiv instead of HTMLElement"<< std::endl;

                        if (!nodeStack.empty())
                        {
                            nodeStack.top()->children.push_back(std::make_shared<HTMLDiv>(pNode));

                            bool isVoid = std::find(voidTags.begin(), voidTags.end(), tagName) != voidTags.end();

                            if (!isSelfClosing && !isVoid && nodeStack.size() < 100)
                                nodeStack.push(nodeStack.top()->children.back().get());
                        }
                    }
                    else if (tagName == L"h1"){
                        HTMLH1 pNode(tagName, attrs, std::vector<std::shared_ptr<HTMLElement>>());
                        std::wcout << "Using HTMLDiv instead of HTMLElement"<< std::endl;

                        if (!nodeStack.empty())
                        {
                            nodeStack.top()->children.push_back(std::make_shared<HTMLH1>(pNode));

                            bool isVoid = std::find(voidTags.begin(), voidTags.end(), tagName) != voidTags.end();

                            if (!isSelfClosing && !isVoid && nodeStack.size() < 100)
                                nodeStack.push(nodeStack.top()->children.back().get());
                        }
                    }
                    else if (tagName == L"h2"){
                        HTMLH2 pNode(tagName, attrs, std::vector<std::shared_ptr<HTMLElement>>());
                        std::wcout << "Using HTMLDiv instead of HTMLElement"<< std::endl;

                        if (!nodeStack.empty())
                        {
                            nodeStack.top()->children.push_back(std::make_shared<HTMLH2>(pNode));

                            bool isVoid = std::find(voidTags.begin(), voidTags.end(), tagName) != voidTags.end();

                            if (!isSelfClosing && !isVoid && nodeStack.size() < 100)
                                nodeStack.push(nodeStack.top()->children.back().get());
                        }
                    }
                    else if (tagName == L"h3"){
                        HTMLH3 pNode(tagName, attrs, std::vector<std::shared_ptr<HTMLElement>>());
                        std::wcout << "Using HTMLDiv instead of HTMLElement"<< std::endl;

                        if (!nodeStack.empty())
                        {
                            nodeStack.top()->children.push_back(std::make_shared<HTMLH3>(pNode));

                            bool isVoid = std::find(voidTags.begin(), voidTags.end(), tagName) != voidTags.end();

                            if (!isSelfClosing && !isVoid && nodeStack.size() < 100)
                                nodeStack.push(nodeStack.top()->children.back().get());
                        }
                    }
                    else
                    {
                        HTMLElement newNode(tagName, attrs, std::vector<std::shared_ptr<HTMLElement>>());

                        if (!nodeStack.empty())
                        {
                            nodeStack.top()->children.push_back(std::make_shared<HTMLElement>(newNode));

                            bool isVoid = std::find(voidTags.begin(), voidTags.end(), tagName) != voidTags.end();

                            if (!isSelfClosing && !isVoid && nodeStack.size() < 100)
                                nodeStack.push(nodeStack.top()->children.back().get());
                        }
                    }
                }
                else
                {
                    if (nodeStack.size() > 1)
                        nodeStack.pop();
                }
            }
            else if (match[4].matched)
            {
                std::wstring text = match[4].str();

                text = std::regex_replace(text, std::wregex(L"\\s+"), std::wstring(L" "));

                size_t first = text.find_first_not_of(L" \t\n\r");
                size_t last = text.find_last_not_of(L" \t\n\r");

                if (first != std::wstring::npos && last != std::wstring::npos)
                {
                    text = text.substr(first, (last - first + 1));

                    if (!text.empty() && !nodeStack.empty())
                    {
                        HTMLElement textNode(std::wstring(L"text"),
                                             std::map<std::wstring, std::wstring>(),
                                             std::vector<std::shared_ptr<HTMLElement>>(),
                                             text);
                        nodeStack.top()->children.push_back(std::make_shared<HTMLElement>(textNode));
                    }
                }
            }
        }

        return root.children;
    }
    catch (const std::exception &e)
    {
        std::wcerr << L"Error in parseHtmlToTree: " << e.what() << std::endl;
        return std::vector<std::shared_ptr<HTMLElement>>();
    }
}