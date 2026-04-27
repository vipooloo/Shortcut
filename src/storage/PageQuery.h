#ifndef PAGE_QUERY_H
#define PAGE_QUERY_H

#include "DbDefine.h"
#include <cstdint>
#include <string>

namespace shortcut {
class PageQuery
{
  public:
    PageQuery()
      : PageQuery("", DbOrderType::ASC, 0U, 0U)
    {}

    PageQuery(const std::string& sortField, DbOrderType orderType, uint32_t pageIndex, uint32_t pageSize)
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

    DbOrderType GetOrderType() const
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

    void SetOrderType(DbOrderType orderType)
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
    DbOrderType m_order_type;
    uint32_t m_page_index;
    uint32_t m_page_size;
};
}  // namespace shortcut

#endif