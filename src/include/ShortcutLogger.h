#ifndef SHORTCUTLOGGER_H
#define SHORTCUTLOGGER_H

#include <cstdarg>
#include <cstdio>

// 颜色定义
#define NONE "\033[0m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"

// INFO 日志（支持 %d %s %f %x 等）
static void logInfo(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    std::printf("%s[INFO]  ", GREEN);
    std::vprintf(format, args);
    std::printf("%s\n", NONE);

    va_end(args);
}

// WARN 日志
static void logWarn(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    std::printf("%s[WARN]  ", YELLOW);
    std::vprintf(format, args);
    std::printf("%s\n", NONE);

    va_end(args);
}

// ERROR 日志
static void logError(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    std::fprintf(stderr, "%s[ERROR] ", RED);
    std::vfprintf(stderr, format, args);
    std::fprintf(stderr, "%s\n", NONE);

    va_end(args);
}

// 对外使用的宏（和你原来一样）
#define LOG_INFO(...) logInfo(__VA_ARGS__)
#define LOG_WARN(...) logWarn(__VA_ARGS__)
#define LOG_ERROR(...) logError(__VA_ARGS__)
#define LOG_WARNING(...) logWarn(__VA_ARGS__)
#define LOG_FORCE_ERROR(...) logError(__VA_ARGS__)
#endif  // SHORTCUTLOGGER_H