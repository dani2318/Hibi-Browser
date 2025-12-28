#pragma once
#include <string>

class Font {
public:
    Font(std::string path, int size) : filePath(path), fontSize(size) {}

    // Add methods your font needs
    std::string getFilePath() const { return filePath; }
    int getSize() const { return fontSize; }

private:
    std::string filePath;
    int fontSize;
};