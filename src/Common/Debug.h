#pragma once
#include <string>

class Debug 
{
public:
    enum struct LogLevel 
    {
        Info,
        Warning,
        Error,
    };

public:
    static void Log(const std::string& message);
    static void LogWarning(const std::string& message);
    static void LogError(const std::string& message);

    static void Assert(bool condition, const std::string& message);

private:
    Debug() = default;

    static void LogInternal(const char* message, const char* file, int line, LogLevel level);
};