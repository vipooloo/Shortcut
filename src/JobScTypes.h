/**
 * @file JobScTypes.h
 * @brief 作业调度系统数据类型定义
 * @details 定义数据库操作相关的数据类型别名
 */

#ifndef JOBSCTYPES_H
#define JOBSCTYPES_H

#include "JobScValue.h"
#include <cstdint>
#include <functional>
#include <map>
#include <string>
#include <vector>

/**
 * @typedef JobScRow
 * @brief 数据库单行记录类型
 */
using JobScRow = std::map<std::string, JobScValue>;

/**
 * @typedef JobScRowList
 * @brief 数据库记录列表类型
 */
using JobScRowList = std::vector<JobScRow>;

/**
 * @typedef UpgradeCallback
 * @brief 数据库升级回调函数类型
 * @param old_version 旧版本号
 * @param new_version 新版本号
 * @return 是否升级成功
 */
using UpgradeCallback = std::function<bool(int64_t, int64_t)>;

#endif  // JOBSCTYPES_H