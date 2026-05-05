/**
 * @file JobScItem.h
 * @brief 作业项数据模型定义
 * @details 定义作业调度系统中的作业实体类，包含作业的所有属性和访问方法
 */

#ifndef JOBSCITEM_H
#define JOBSCITEM_H

#include "JobScDefines.h"
#include <cstdint>
#include <string>
#include <vector>

/**
 * @class JobScItem
 * @brief 作业项实体类
 * @details 表示一个具体的作业任务，包含作业的所有属性信息
 */
class JobScItem
{
  public:
    JobScItem()
      : m_rid{INVALID_RID}
      , m_account_id{0}
      , m_job_type{JobScType::None}
      , m_description{""}
      , m_settings{}
      , m_address_list{}
    {}
    ~JobScItem() = default;
    JobScItem(const JobScItem& other)
    {
        if (this != &other)
        {
            m_rid = other.m_rid;
            m_account_id = other.m_account_id;
            m_job_type = other.m_job_type;
            m_description = other.m_description;
            m_settings = other.m_settings;
            m_address_list = other.m_address_list;
        }
    }
    JobScItem& operator=(const JobScItem&) = delete;
    JobScItem& operator=(const JobScItem&& other) noexcept
    {
        if (this != &other)
        {
            m_rid = other.m_rid;
            m_account_id = other.m_account_id;
            m_job_type = other.m_job_type;
            m_description = std::move(other.m_description);
            m_settings = std::move(other.m_settings);
            m_address_list = std::move(other.m_address_list);
        }
        return *this;
    }
    JobScItem(const JobScItem&& other) noexcept
    {
        if (this != &other)
        {
            m_rid = other.m_rid;
            m_account_id = other.m_account_id;
            m_job_type = other.m_job_type;
            m_description = std::move(other.m_description);
            m_settings = std::move(other.m_settings);
            m_address_list = std::move(other.m_address_list);
        }
    }

    /**
     * @brief 获取记录ID
     * @return 记录ID
     */
    int64_t GetRid() const
    {
        return m_rid;
    }

    /**
     * @brief 获取账号ID
     * @return 账号ID
     */
    uint64_t GetAccountId() const
    {
        return m_account_id;
    }

    /**
     * @brief 获取作业类型
     * @return 作业类型
     */
    JobScType GetJobType() const
    {
        return m_job_type;
    }

    /**
     * @brief 获取描述
     * @return 作业描述
     */
    const std::string& GetDescription() const
    {
        return m_description;
    }

    /**
     * @brief 获取设置
     * @return 作业设置
     */
    const std::vector<uint8_t>& GetSettings() const
    {
        return m_settings;
    }

    /**
     * @brief 获取地址列表
     * @return 地址列表
     */
    const std::vector<uint8_t>& GetAddressList() const
    {
        return m_address_list;
    }
    /**
     * @brief 设置记录ID
     * @param rid 记录ID
     */
    void SetRid(int64_t rid)
    {
        m_rid = rid;
    }

    /**
     * @brief 设置账号ID
     * @param account_id 账号ID
     */
    void SetAccountId(uint64_t account_id)
    {
        m_account_id = account_id;
    }

    /**
     * @brief 设置作业类型
     * @param job_type 作业类型
     */
    void SetJobType(JobScType job_type)
    {
        m_job_type = job_type;
    }

    /**
     * @brief 设置描述
     * @param description 作业描述
     */
    void SetDescription(const std::string& description)
    {
        m_description = description;
    }

    /**
     * @brief 设置作业设置
     * @param settings 作业设置
     */
    void SetSettings(const std::vector<uint8_t>& settings)
    {
        m_settings = settings;
    }

    /**
     * @brief 设置地址列表
     * @param address_list 地址列表
     */
    void SetAddressList(const std::vector<uint8_t>& address_list)
    {
        m_address_list = address_list;
    }

  private:
    int64_t m_rid;                        ///< 主键
    uint64_t m_account_id;                ///< 账号ID
    JobScType m_job_type;                 ///< 作业类型
    std::string m_description;            ///< 描述
    std::vector<uint8_t> m_settings;      ///< 作业设置
    std::vector<uint8_t> m_address_list;  ///< 地址列表
};

#endif  // JOBSCITEM_H