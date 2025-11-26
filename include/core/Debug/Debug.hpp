#pragma once
#define NOMINMAX
#include <windows.h>
#include <mutex>
#include <fstream>

class Logger {
public:
    void Log(const std::string &msg);
private:
    std::mutex mutex_;
};
