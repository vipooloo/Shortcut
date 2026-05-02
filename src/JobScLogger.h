#ifndef JOBSCLOGGER_H
#define JOBSCLOGGER_H

#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <string>

// ==================== 颜色定义 ====================
#define LOG_COLOR_NONE "\033[0m"
#define LOG_COLOR_RED "\033[31m"
#define LOG_COLOR_GREEN "\033[32m"
#define LOG_COLOR_YELLOW "\033[33m"

// ==================== 工具函数：获取时间戳 ====================
// 格式：YYYY-MM-DD HH:MM:SS.sss
static std::string getLogTimestamp()
{
    // 获取系统时间
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

    // 获取毫秒部分
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    // 格式化时间
    char timeBuf[32];
    std::strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", std::localtime(&nowTime));

    // 拼接毫秒
    char timestamp[40];
    snprintf(timestamp, sizeof(timestamp), "%s.%03lu", timeBuf, static_cast<unsigned long>(ms.count()));

    return timestamp;
}

// ==================== 统一日志打印函数（减少重复代码） ====================
static void logPrint(const char* level, const char* color, FILE* stream, const char* file, int line, const char* format, va_list args)
{
    // 输出格式：[时间戳] [级别] [文件:行号] 内容
    std::fprintf(stream, "%s[%s] [%s] [%s:%d]  ", color, getLogTimestamp().c_str(), level, file, line);
    std::vfprintf(stream, format, args);
    std::fprintf(stream, "%s\n", LOG_COLOR_NONE);
}

// ==================== 日志接口实现 ====================
static void logInfo(const char* file, int line, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    logPrint("INFO", LOG_COLOR_GREEN, stdout, file, line, format, args);
    va_end(args);
}

static void logWarn(const char* file, int line, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    logPrint("WARN", LOG_COLOR_YELLOW, stdout, file, line, format, args);
    va_end(args);
}

static void logError(const char* file, int line, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    logPrint("ERROR", LOG_COLOR_RED, stderr, file, line, format, args);
    va_end(args);
}

// ==================== 对外宏定义（兼容原有用法） ====================
#define JOBSC_LOG_INFO(...) logInfo(__FILE__, __LINE__, __VA_ARGS__)
#define JOBSC_LOG_WARN(...) logWarn(__FILE__, __LINE__, __VA_ARGS__)
#define JOBSC_LOG_ERROR(...) logError(__FILE__, __LINE__, __VA_ARGS__)
#define JOBSC_LOG_FORCE(...) JOBSC_LOG_ERROR(__VA_ARGS__)

#endif  // JOBSCLOGGER_H