#include "Logger.h"

void Logger::log(Level level, const std::string& message)
{
    std::cerr << "[" << levelToString(level) << "]" << message << std::endl;
}

const char* Logger::levelToString(Level level)
{
    switch (level)
    {
    case Level::INFO: return "INFO";
    case Level::WARN: return "WARN";
    case Level::ERROR: return "ERROR";
    default: return "UNKNOWN";
    }
}
