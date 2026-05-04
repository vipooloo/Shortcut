#ifndef JOBSCPAGEQUERY_H
#define JOBSCPAGEQUERY_H

#include "JobScDefines.h"
#include <cstdint>
#include <string>

class JobScPageQuery
{
  public:
    JobScPageQuery()
      : m_keyword{""}
      , m_type{JobScType::None}
      , m_order_type{JobScOrderType::ASC}
      , m_page_index{JOBSC_PAGE_DEFAULT_INDEX}
      , m_page_size{JOBSC_PAGE_DEFAULT_SIZE}
    {}

    ~JobScPageQuery() = default;

    const std::string& GetKeyword() const
    {
        return m_keyword;
    }
    void SetKeyword(const std::string& keyword)
    {
        m_keyword = keyword;
    }
    JobScType GetType() const
    {
        return m_type;
    }
    void SetType(JobScType type)
    {
        m_type = type;
    }
    JobScOrderType GetOrderType() const
    {
        return m_order_type;
    }
    void SetOrderType(JobScOrderType order_type)
    {
        m_order_type = order_type;
    }
    uint32_t GetPageIndex() const
    {
        return m_page_index;
    }
    void SetPageIndex(uint32_t page_index)
    {
        m_page_index = page_index;
    }
    uint32_t GetPageSize() const
    {
        return m_page_size;
    }
    void SetPageSize(uint32_t page_size)
    {
        m_page_size = page_size;
    }

  private:
    std::string m_keyword;
    JobScType m_type;
    JobScOrderType m_order_type;
    uint32_t m_page_index;
    uint32_t m_page_size;
};

#endif  // JOBSCPAGEQUERY_H