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
      : m_sortField(sortField)
      , m_orderType(orderType)
      , m_pageIndex(pageIndex)
      , m_pageSize(pageSize)
    {
    }

    std::string GetSortField() const
    {
        return m_sortField;
    }

    const char* GetSortFieldCStr() const
    {
        return m_sortField.c_str();
    }

    DbOrderType GetOrderType() const
    {
        return m_orderType;
    }

    uint32_t GetPageIndex() const
    {
        return m_pageIndex;
    }

    uint32_t GetPageSize() const
    {
        return m_pageSize;
    }

    void SetSortField(const std::string& sortField)
    {
        m_sortField = sortField;
    }

    void SetOrderType(DbOrderType orderType)
    {
        m_orderType = orderType;
    }

    void SetPageIndex(uint32_t pageIndex)
    {
        m_pageIndex = pageIndex;
    }

    void SetPageSize(uint32_t pageSize)
    {
        m_pageSize = pageSize;
    }

  private:
    std::string m_sortField;
    DbOrderType m_orderType;
    uint32_t m_pageIndex;
    uint32_t m_pageSize;
};
}  // namespace shortcut

#endif