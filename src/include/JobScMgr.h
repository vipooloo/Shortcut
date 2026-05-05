/**
 * @file JobScMgr.h
 * @brief 作业调度管理器接口
 * @details 提供作业调度系统的静态方法接口，作为对外的统一入口
 */

#ifndef JOBSCMGR_H
#define JOBSCMGR_H

#include "JobScItem.h"
#include "JobScPageQuery.h"
#include "JobScResult.h"
#include <cstdint>

/**
 * @class JobScMgr
 * @brief 作业调度管理器
 * @details 提供作业增删改查等操作的静态接口
 */
class JobScMgr
{
  public:
    /**
     * @brief 作业观察者回调函数类型
     * 
     * @param account_id 账号ID
     * @param type 类型
     * @param description  描述信息
     * @param job_settings 作业参数
     * @param addr_info    地址参数
     * @return std::pair<JobScResult, JobScItem> 
     */
    static std::pair<JobScResult, JobScItem> Add(uint64_t account_id,
                                                 JobScType type,
                                                 const std::string& description,
                                                 const std::vector<uint8_t>& job_settings,
                                                 const std::vector<uint8_t>& addr_info);

    /**
     * @brief 删除作业
     * @param rids 作业记录ID列表
     * @return 操作结果
     */
    static JobScResult Delete(const std::vector<int64_t>& rids);

    /**
     * @brief 按类型删除作业
     * @param types 作业类型列表
     * @return 操作结果
     */
    static JobScResult DeleteByType(const std::vector<JobScType>& types);

    /**
     * @brief 更新作业
     * @param rid 作业记录ID
     * @param item 作业项
     * @return 操作结果
     */
    static JobScResult Update(int64_t rid, const JobScItem& item);

    /**
     * @brief 分页查询作业列表
     * @param page_query 分页查询条件
     * @param out_result 输出分页结果
     * @param out_items 输出作业列表
     * @return 操作结果
     */
    static JobScResult Query(const JobScPageQuery& page_query, JobScPageResult& out_result, std::vector<JobScItem>& out_items);

    /**
     * @brief 添加作业观察者
     * @param observer 观察者回调函数
     */
    static void AddObserver(const JobScObserver& observer);

    /**
     * @brief 添加作业观察者
     * @param observer 观察者回调函数
     */
    static void RemoveObserver(const JobScObserver& observer);
};

#endif  // JOBSCMGR_H