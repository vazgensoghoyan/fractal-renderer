#pragma once // utils/logger.hpp

#include <iostream>
#include <mutex>
#include <chrono>
#include <string>
#include <format>

#define LOGGING_ENABLED

#ifdef LOGGING_ENABLED
    #define LOG_INFO(fmt, ...)  iheay::utils::Logger::info(fmt, ##__VA_ARGS__)
    #define LOG_WARN(fmt, ...)  iheay::utils::Logger::warn(fmt, ##__VA_ARGS__)
    #define LOG_DEBUG(fmt, ...) iheay::utils::Logger::debug(fmt, ##__VA_ARGS__)
    #define LOG_ERROR(fmt, ...) iheay::utils::Logger::error(fmt, ##__VA_ARGS__)
#else
    #define LOG_INFO(fmt, ...)  ((void)0)
    #define LOG_WARN(fmt, ...)  ((void)0)
    #define LOG_DEBUG(fmt, ...) ((void)0)
    #define LOG_ERROR(fmt, ...) ((void)0)
#endif

namespace iheay::utils {

enum class LogLevel { Info, Warn, Debug, Error };

class Logger {
public:
    Logger() = delete;

    template<typename... Args>
    static void info(std::format_string<Args...> fmt, Args&&... args) {
        log(LogLevel::Info, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void warn(std::format_string<Args...> fmt, Args&&... args) {
        log(LogLevel::Warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void debug(std::format_string<Args...> fmt, Args&&... args) {
        log(LogLevel::Debug, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void error(std::format_string<Args...> fmt, Args&&... args) {
        log(LogLevel::Error, fmt, std::forward<Args>(args)...);
    }

private:
    static inline std::mutex m_mutex;

    template<typename... Args>
    static void log(LogLevel level, std::format_string<Args...> fmt, Args&&... args) {
        const char* color = "\033[0m";
        std::string tag;

        switch(level) {
            case LogLevel::Info: color = "\033[32m"; tag = "INFO"; break;    // green
            case LogLevel::Warn: color = "\033[33m"; tag = "WARN"; break;    // yellow
            case LogLevel::Debug: color = "\033[36m"; tag = "DEBUG"; break;  // cyan
            case LogLevel::Error: color = "\033[31m"; tag = "ERROR"; break;  // red
        }

        auto now = std::chrono::system_clock::now();
        auto t_c = std::chrono::system_clock::to_time_t(now);
        std::tm tm{};
#if defined(_WIN32)
        localtime_s(&tm, &t_c);
#else
        localtime_r(&t_c, &tm);
#endif

        std::string message = std::format(fmt, std::forward<Args>(args)...);

        std::lock_guard<std::mutex> lock(m_mutex);
        std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
                  << " " << color << "[" << tag << "]\033[0m "
                  << message << std::endl;
    }
};

} // namespace iheay::utils
