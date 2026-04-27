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
// 增加 file 和 line 参数
static void logInfo(const char* file, int line, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    // 打印颜色、级别、文件名和行号
    std::printf("%s[INFO] [%s:%d]  ", GREEN, file, line);
    std::vprintf(format, args);
    std::printf("%s\n", NONE);

    va_end(args);
}

// WARN 日志
static void logWarn(const char* file, int line, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    std::printf("%s[WARN] [%s:%d]  ", YELLOW, file, line);
    std::vprintf(format, args);
    std::printf("%s\n", NONE);

    va_end(args);
}

// ERROR 日志
static void logError(const char* file, int line, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    std::fprintf(stderr, "%s[ERROR] [%s:%d] ", RED, file, line);
    std::vfprintf(stderr, format, args);
    std::fprintf(stderr, "%s\n", NONE);

    va_end(args);
}

// 对外使用的宏
// 在用户调用 LOG_INFO(...) 时，自动将当前的文件名和行号传给实际函数
#define LOG_INFO(...) logInfo(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...) logWarn(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) logError(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARNING(...) logWarn(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_FORCE(...) logError(__FILE__, __LINE__, __VA_ARGS__)

#endif  // SHORTCUTLOGGER_H