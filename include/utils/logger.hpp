#pragma once
#include <string>
#include <fstream>
#include <iostream>

namespace utils {
    enum class LogLevel { DEBUG = 0, INFO = 1, WARN = 2, ERROR = 3 };

    class Logger {
    public:
        static Logger& instance();
        void setLevel(LogLevel level) { minLevel = level; }
        void setFile(const std::string& path);
        void log(LogLevel level, const std::string& msg);
        void debug(const std::string& msg) { log(LogLevel::DEBUG, msg); }
        void info (const std::string& msg) { log(LogLevel::INFO,  msg); }
        void warn (const std::string& msg) { log(LogLevel::WARN,  msg); }
        void error(const std::string& msg) { log(LogLevel::ERROR, msg); }

    private:
        Logger() = default;
        LogLevel     minLevel = LogLevel::INFO;
        std::ofstream fileOut;
        static const char* levelStr(LogLevel l);
    };

    inline Logger& Log() { return Logger::instance(); }
}
