#pragma once // logger.hpp

#include <iostream>
#include <mutex>
#include <string>
#include <chrono>
#include <iomanip>
#include <cstdarg>
#include <vector>

#define LOG_INFO(fmt, ...) iheay::utils::Logger::info(fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) iheay::utils::Logger::debug(fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) iheay::utils::Logger::error(fmt, ##__VA_ARGS__)

namespace iheay::utils {

class Logger {
public:
    Logger() = delete;

    static void info(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        log("INFO", fmt, args);
        va_end(args);
    }

    static void debug(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        log("DEBUG", fmt, args);
        va_end(args);
    }

    static void error(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        log("ERROR", fmt, args);
        va_end(args);
    }

private:
    static std::mutex m_mutex;

    static void log(const std::string& tag, const char* fmt, va_list args) {
        va_list args_copy;
        va_copy(args_copy, args);
        int size = std::vsnprintf(nullptr, 0, fmt, args_copy);
        va_end(args_copy);

        std::vector<char> buffer(size + 1);
        std::vsnprintf(buffer.data(), buffer.size(), fmt, args);

        auto now = std::chrono::system_clock::now();
        auto t_c = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
#if defined(_WIN32)
        localtime_s(&tm, &t_c);
#else
        localtime_r(&t_c, &tm);
#endif

        std::lock_guard<std::mutex> lock(m_mutex);
        std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
                  << " [" << tag << "] " << buffer.data() << std::endl;
    }
};

std::mutex Logger::m_mutex;

} // namespace iheay::util
