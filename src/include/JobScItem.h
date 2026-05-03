#ifndef JOBSCITEM_H
#define JOBSCITEM_H

#include "JobScDefines.h"
#include <cstdint>
#include <string>
#include <vector>

class JobScItem
{
  public:
    JobScItem()
      : m_rid{-1}
      , m_account_id{0}
      , m_job_type{JobScType::None}
      , m_description{""}
      , m_settings{}
      , m_address_list{}
    {}
    ~JobScItem() = default;
    JobScItem(const JobScItem&) = delete;
    JobScItem& operator=(const JobScItem&) = delete;

    int64_t GetRid() const
    {
        return m_rid;
    }
    uint64_t GetAccountId() const
    {
        return m_account_id;
    }
    JobScType GetJobType() const
    {
        return m_job_type;
    }
    const std::string& GetDescription() const
    {
        return m_description;
    }
    const std::vector<uint8_t>& GetSettings() const
    {
        return m_settings;
    }
    const std::vector<uint8_t>& GetAddressList() const
    {
        return m_address_list;
    }
    void SetRid(int64_t rid)
    {
        m_rid = rid;
    }
    void SetAccountId(uint64_t account_id)
    {
        m_account_id = account_id;
    }
    void SetJobType(JobScType job_type)
    {
        m_job_type = job_type;
    }
    void SetDescription(const std::string& description)
    {
        m_description = description;
    }
    void SetSettings(const std::vector<uint8_t>& settings)
    {
        m_settings = settings;
    }
    void SetAddressList(const std::vector<uint8_t>& address_list)
    {
        m_address_list = address_list;
    }

  private:
    int64_t m_rid;                        // 主键
    uint64_t m_account_id;                // 账号ID
    JobScType m_job_type;                 // 任务类型
    std::string m_description;            // 描述
    std::vector<uint8_t> m_settings;      // 设置
    std::vector<uint8_t> m_address_list;  // 地址列表
};

#endif  // JOBSCITEM_H