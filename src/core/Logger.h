#pragma once
#include <iostream>
#include <string>
#include <sstream>

class Logger
{
public:
    enum class Level { INFO, WARN, ERROR };

    static void log(Level level, const std::string& message);

    template<typename... Args>
    static void info(Args&&... args)
    {
        log(Level::INFO, format(std::forward<Args>(args)...));
    }

    template<typename... Args>
    static void warn(Args&&... args)
    {
        log(Level::WARN, format(std::forward<Args>(args)...));
    }

    template<typename... Args>
    static void error(Args&&... args)
    {
        log(Level::ERROR, format(std::forward<Args>(args)...));
    }

private:
    template<typename... Args>
    static std::string format(Args&&... args)
    {
        std::ostringstream stream;
        (stream << ... << args);
        return stream.str();
    }

    static const char* levelToString(Level level);
};
