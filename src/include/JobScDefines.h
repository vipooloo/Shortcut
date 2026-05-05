/**
 * @file JobScDefines.h
 * @brief 作业调度系统核心类型定义
 * @details 定义作业调度系统中使用的枚举类型、结构体和常量
 */

#ifndef JOBSCDEFINES_H
#define JOBSCDEFINES_H

#include <cstdint>
#include <functional>

/**
 * @enum JobScOrderType
 * @brief 排序类型枚举
 */
enum class JobScOrderType : int32_t
{
    ASC = 0,
    DESC = 1
};

/**
 * @struct JobScPageResult
 * @brief 分页查询结果结构体
 */
typedef struct JobScPageResult
{
    uint32_t total_count;  ///< 总记录数
    uint32_t total_page;   ///< 总页数
    uint32_t page_index;   ///< 当前页码
    uint32_t page_size;    ///< 每页大小
} JOBSCPAGERESULT_S;

/**
 * @enum JobScType
 * @brief 作业类型枚举
 */
enum class JobScType : int32_t
{
    None = 0,
    ScanToEmail,
    ScanToFTP,
    ScanToSMB,
    ScanToUSB,
    All,
};

/**
 * @enum JobScEventType
 * @brief 作业事件类型枚举
 */
enum class JobScEventType : int32_t
{
    Added,    ///< 新增事件
    Updated,  ///< 修改事件
    Deleted,  ///< 删除事件
};

/**
 * @typedef JobScObserver
 * @brief 作业观察者回调函数类型
 * @param event 作业事件类型
 */
using JobScObserver = std::function<void(JobScEventType event)>;

/**
 * @brief 默认分页起始页码
 */

static constexpr uint32_t JOBSC_PAGE_DEFAULT_INDEX = 0U;

/**
 * @brief 默认分页每页大小
 */
static constexpr uint32_t JOBSC_PAGE_DEFAULT_SIZE = 10U;

#endif  // JOBSCDEFINES_H