#include "utils/logger.hpp"

namespace utils {
    Logger& Logger::instance() {
        static Logger inst;
        return inst;
    }

    void Logger::setFile(const std::string& path) {
        fileOut.open(path, std::ios::app);
    }

    void Logger::log(LogLevel level, const std::string& msg) {
        if (level < minLevel) return;
        std::string line = std::string("[") + levelStr(level) + "] " + msg + "\n";
        std::cerr << line;
        if (fileOut.is_open()) fileOut << line;
    }

    const char* Logger::levelStr(LogLevel l) {
        switch (l) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO:  return "INFO";
            case LogLevel::WARN:  return "WARN";
            case LogLevel::ERROR: return "ERROR";
        }
        return "?";
    }
}
