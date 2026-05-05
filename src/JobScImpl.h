/**
 * @file JobScImpl.h
 * @brief 作业调度实现类定义
 * @details 定义作业调度系统的核心实现类，提供具体的业务逻辑
 */

#ifndef JOBSCIMPL_H
#define JOBSCIMPL_H

#include "JobScDao.h"
#include "JobScInitializer.h"
#include "JobScItem.h"
#include "JobScPageQuery.h"
#include "JobScResult.h"
#include <list>
#include <mutex>

/**
 * @class JobScImpl
 * @brief 作业调度实现类
 * @details 单例模式实现，负责作业的增删改查和观察者通知
 */
class JobScImpl
{
  public:
    /**
     * @brief 获取单例实例
     * @return 单例引用
     */
    static JobScImpl& GetInstance();

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
    std::pair<JobScResult, JobScItem> Add(uint64_t account_id,
                                          JobScType type,
                                          const std::string& description,
                                          const std::vector<uint8_t>& job_settings,
                                          const std::vector<uint8_t>& addr_info);

    /**
     * @brief 添加作业
     * @param account_id 账号ID
     * @param item 作业项
     * @return 操作结果和新插入记录ID
     */
    std::pair<JobScResult, int64_t> Add(uint64_t account_id, const JobScItem& item);

    /**
     * @brief 删除作业
     * @param rids 作业记录ID列表
     * @return 操作结果
     */
    JobScResult Delete(const std::vector<int64_t>& rids);

    /**
     * @brief 按类型删除作业
     * @param types 作业类型列表
     * @return 操作结果
     */
    JobScResult DeleteByType(const std::vector<JobScType>& types);

    /**
     * @brief 更新作业
     * @param rid 作业记录ID
     * @param item 作业项
     * @return 操作结果
     */
    JobScResult Update(int64_t rid, const JobScItem& item);

    /**
     * @brief 分页查询作业列表
     * @param page_query 分页查询条件
     * @param out_result 输出分页结果
     * @param out_items 输出作业列表
     * @return 操作结果
     */
    JobScResult GetListByTypePage(
        const JobScPageQuery& page_query,
        JobScPageResult& out_result,
        std::vector<JobScItem>& out_items);

    /**
     * @brief 添加作业观察者
     * @param observer 观察者回调函数
     */
    void AddObserver(const JobScObserver& observer);

    /**
     * @brief 添加作业观察者
     * @param observer 观察者回调函数
     */
    void RemoveObserver(const JobScObserver& observer);

  private:
    /**
     * @brief 私有构造函数
     */
    JobScImpl();
    ~JobScImpl() = default;
    JobScImpl(const JobScImpl&) = delete;
    JobScImpl& operator=(const JobScImpl&) noexcept = delete;
    JobScImpl(JobScImpl&&) = delete;
    JobScImpl& operator=(JobScImpl&&) noexcept = delete;
    /**
     * @brief 通知所有观察者
     * @param event 作业事件类型
     */
    void Notify(JobScEventType event);
    /**
     * @brief 将数据库行转换为作业项列表
     * @param rows 数据库行列表
     * @return 作业项列表
     */
    std::vector<JobScItem> CovertrToItems(const JobScRowList& rows);

  private:
    std::mutex m_service_mutex;            ///< 服务互斥锁
    JobScInitializer m_initializer;        ///< 数据库初始化器
    JobScDao m_dao;                        ///< 数据访问对象
    std::mutex m_observers_mutex;          ///< 观察者列表互斥锁
    std::list<JobScObserver> m_observers;  ///< 观察者列表
};

#endif  // JOBSCIMPL_H