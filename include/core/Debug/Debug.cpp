#include <core/Debug/Debug.hpp>

void Logger::Log(const std::string &msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::ofstream log("hibi_browser.log", std::ios::app);
        SYSTEMTIME st{};
        GetLocalTime(&st);
        log << "[" << st.wHour << ":" << st.wMinute << ":" << st.wSecond << "] " << msg << "\n";
}