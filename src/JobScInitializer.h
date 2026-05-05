/**
 * @file JobScInitializer.h
 * @brief 数据库初始化器定义
 * @details 定义数据库初始化和升级相关功能
 */

#ifndef JOBSCINITIALIZER_H
#define JOBSCINITIALIZER_H

#include "JobScDao.h"
#include <cstdint>

class JobScDbAccess;

/**
 * @class JobScInitializer
 * @brief 数据库初始化器
 * @details 负责数据库初始化和版本升级工作
 */
class JobScInitializer
{
  public:
    JobScInitializer() = default;
    ~JobScInitializer() = default;

    /**
     * @brief 初始化数据库
     * @param dao 数据访问对象
     */
    void Init(JobScDao& dao);

    /**
     * @brief 升级数据库
     * @param db_access_ptr 数据库访问指针
     * @param old_ver 旧版本
     * @param new_ver 新版本
     * @return 是否升级成功
     */
    bool Upgrade(const std::shared_ptr<JobScDbAccess>& db_access_ptr, int64_t old_ver, int64_t new_ver);
};

#endif  // JOBSCINITIALIZER_H