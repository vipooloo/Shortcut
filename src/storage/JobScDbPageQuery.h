#ifndef JOBSCDBPAGEQUERY_H
#define JOBSCDBPAGEQUERY_H

#include "JobScDefines.h"
#include <cstdint>
#include <string>

class JobScDbPageQuery
{
  public:
    JobScDbPageQuery()
      : JobScDbPageQuery("", JobScOrderType::ASC, JOBSC_PAGE_DEFAULT_INDEX, JOBSC_PAGE_DEFAULT_SIZE)
    {}

    JobScDbPageQuery(const std::string& sortField, JobScOrderType orderType, uint32_t pageIndex, uint32_t pageSize)
      : m_sort_field(sortField)
      , m_order_type(orderType)
      , m_page_index(pageIndex)
      , m_page_size(pageSize)
    {
    }

    std::string GetSortField() const
    {
        return m_sort_field;
    }

    const char* GetSortFieldCStr() const
    {
        return m_sort_field.c_str();
    }

    JobScOrderType GetOrderType() const
    {
        return m_order_type;
    }

    uint32_t GetPageIndex() const
    {
        return m_page_index;
    }

    uint32_t GetPageSize() const
    {
        return m_page_size;
    }

    void SetSortField(const std::string& sortField)
    {
        m_sort_field = sortField;
    }

    void SetOrderType(JobScOrderType orderType)
    {
        m_order_type = orderType;
    }

    void SetPageIndex(uint32_t pageIndex)
    {
        m_page_index = pageIndex;
    }

    void SetPageSize(uint32_t pageSize)
    {
        m_page_size = pageSize;
    }

  private:
    std::string m_sort_field;
    JobScOrderType m_order_type;
    uint32_t m_page_index;
    uint32_t m_page_size;
};

#endif  // JOBSCDBPAGEQUERY_H