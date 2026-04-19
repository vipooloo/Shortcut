#ifndef SHORTCULOGGER_H
#define SHORTCULOGGER_H

#include <iostream>
#include <string>

// 开启 Windows 控制台颜色（自动识别平台）
#ifdef _WIN32
#include <windows.h>
static void
enableWinColor()
{
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode = 0;
  GetConsoleMode(hOut, &mode);
  SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
static bool colorInit = []() {
  enableWinColor();
  return true;
}();
#endif

// 颜色定义
#define NONE "\033[0m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define GREEN "\033[32m"

// ==============================================
// 核心：打印任意多个参数（逗号分隔，不用 <<）
// ==============================================
template<typename... Args>
static void
logInfo(Args&&... args)
{
  std::cout << GREEN "[INFO]  ";
  (std::cout << ... << args) << NONE << '\n';
}

template<typename... Args>
static void
logWarn(Args&&... args)
{
  std::cout << YELLOW "[WARN]  ";
  (std::cout << ... << args) << NONE << '\n';
}

template<typename... Args>
static void
logError(Args&&... args)
{
  std::cerr << RED "[ERROR] ";
  (std::cout << ... << args) << NONE << '\n';
}

// 对外宏，逗号传参！
#define LOG_INFO(...) logInfo(__VA_ARGS__)
#define LOG_WARN(...) logWarn(__VA_ARGS__)
#define LOG_ERROR(...) logError(__VA_ARGS__)
#define LOG_WARNING(...) logWarn(__VA_ARGS__)
#endif // SHORTCULOGGER_H