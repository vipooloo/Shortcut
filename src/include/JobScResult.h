/**
 * @file JobScResult.h
 * @brief 作业调度系统操作结果定义
 */

#ifndef JOBSCRESULT_H
#define JOBSCRESULT_H

#include <cstdint>

/**
 * @enum JobScResult
 * @brief 作业调度操作结果枚举
 */
enum class JobScResult : int32_t
{
    Success = 0,      ///< 成功
    Failed,           ///< 失败
    MaxCountReached,  ///< 超过最大限制
    AlreadyExist,     ///< 已存在
    InvalidParam,     ///< 参数错误
};

#endif  // JOBSCRESULT_H