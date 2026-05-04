#ifndef JOBSCRESULT_H
#define JOBSCRESULT_H

#include <cstdint>

enum class JobScResult : int32_t
{
    Success = 0,      ///< 成功
    Failed,           ///< 失败
    MaxCountReached,  ///< 超过最大限制
    NoExist,          ///< 记录不存在
    AlreadyExist,     ///< 已存在
    InvalidParam,     ///< 参数错误
};

#endif  // JOBSCRESULT_H