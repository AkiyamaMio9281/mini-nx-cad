#pragma once

#include <iostream>
#include <string_view>

namespace mnx {

enum class LogLevel {
    Info,
    Warning,
    Error,
};

class Logger {
public:
    static void log(LogLevel level, std::string_view message) {
        std::ostream& stream = level == LogLevel::Error ? std::cerr : std::clog;
        stream << label(level) << ": " << message << '\n';
    }

private:
    static constexpr std::string_view label(LogLevel level) {
        switch(level) {
        case LogLevel::Info:
            return "info";
        case LogLevel::Warning:
            return "warning";
        case LogLevel::Error:
            return "error";
        }
        return "unknown";
    }
};

} // namespace mnx
