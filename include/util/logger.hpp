#pragma once
#include <iostream>
#include <mutex>
#include <string>
#include <chrono>
#include <iomanip>

namespace iheay::util {

class Logger {
public:
    Logger() = delete;

    static void info(const std::string& msg) {
        log("INFO", msg);
    }

    static void debug(const std::string& msg) {
        log("DEBUG", msg);
    }

    static void error(const std::string& msg) {
        log("ERROR", msg);
    }

private:
    static std::mutex m_mutex;

    static void log(const std::string& tag, const std::string& msg) {
        auto now = std::chrono::system_clock::now();
        auto t_c = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::localtime(&t_c);

        std::lock_guard<std::mutex> lock(m_mutex);
        std::cout << std::put_time(&tm, "%Y-%m-%d %H:%M:%S")
                  << " [" << tag << "] " << msg << std::endl;
    }
};

} // namespace iheay::log
