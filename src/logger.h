#pragma once
#include <stdint.h>

namespace RSCV
{
    enum LogType : uint8_t
    {
        TRACE,
        INFO,
        DEBUG,
        WARN,
        ERROR,
        CRITICAL
    };

    class Logger
    {
    public:
        static void Log(LogType type, const char* format, ...);
    };
}



#ifdef NDEBUG
    #define RSCV_LOG_TRACE(msg, ...)
    #define RSCV_LOG_INFO(msg, ...)
    #define RSCV_LOG_DEBUG(msg, ...)
    #define RSCV_LOG_WARN(msg, ...)
    #define RSCV_LOG_ERROR(msg, ...)
    #define RSCV_LOG_CRITICAL(msg, ...)
#else
    #define RSCV_LOG_TRACE(msg, ...)    Logger::Log(LogType::TRACE, msg, ##__VA_ARGS__)
    #define RSCV_LOG_INFO(msg, ...)     Logger::Log(LogType::INFO, msg, ##__VA_ARGS__)
    #define RSCV_LOG_DEBUG(msg, ...)    Logger::Log(LogType::DEBUG, msg, ##__VA_ARGS__)
    #define RSCV_LOG_WARN(msg, ...)     Logger::Log(LogType::WARN, msg, ##__VA_ARGS__)
    #define RSCV_LOG_ERROR(msg, ...)    Logger::Log(LogType::ERROR, msg, ##__VA_ARGS__)
    #define RSCV_LOG_CRITICAL(msg, ...) Logger::Log(LogType::CRITICAL, msg, ##__VA_ARGS__)
#endif