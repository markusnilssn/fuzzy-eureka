#include "Debug.h"
#include <cassert>
#include <iostream>

void Debug::Log(const std::string& message)
{
    LogInternal(message.c_str(), __FILE__, __LINE__, LogLevel::Info);
}

void Debug::LogWarning(const std::string& message)
{
    LogInternal(message.c_str(), __FILE__, __LINE__, LogLevel::Warning);
}

void Debug::LogError(const std::string& message)
{
    LogInternal(message.c_str(), __FILE__, __LINE__, LogLevel::Error);
}

void Debug::Assert(bool condition, const std::string& message)
{
    if (!condition)
    {
        LogInternal(message.c_str(), __FILE__, __LINE__, LogLevel::Error);
        assert(condition);
    }
}

static std::string GetCurrentTime()
{
    std::time_t now = std::time(nullptr);
    std::tm *tm = std::localtime(&now);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
    return buffer;
}

static std::string GetLogLevelString(Debug::LogLevel level)
{
    switch (level)
    {
        case Debug::LogLevel::Info:
            return "[INFO]";
        case Debug::LogLevel::Warning:
            return "[WARNING]";
        case Debug::LogLevel::Error:
            return "[ERROR]";
        default:
            return "[UNKNOWN]";
    }
}

void Debug::LogInternal(const char *message, const char *file, int line, LogLevel level)
{
    std::string logLevelString = GetLogLevelString(level);
    std::string currentTime = GetCurrentTime();

    std::cout << currentTime << " " << logLevelString << " " << file << ":" << line << " - " << message << std::endl;
}
