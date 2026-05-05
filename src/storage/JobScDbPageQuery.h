/**
 * @file JobScDbPageQuery.h
 * @brief 数据库分页查询条件定义
 * @details 用于封装数据库分页查询的具体参数，如排序字段、排序方式、页码等
 */

#ifndef JOBSCDBPAGEQUERY_H
#define JOBSCDBPAGEQUERY_H

#include "JobScDefines.h"
#include <cstdint>
#include <string>

/**
 * @class JobScDbPageQuery
 * @brief 数据库分页查询条件类
 * @details 提供数据库分页查询所需的各种参数
 */
class JobScDbPageQuery
{
  public:
    /**
     * @brief 默认构造函数
     */
    JobScDbPageQuery()
      : JobScDbPageQuery("", JobScOrderType::ASC, JOBSC_PAGE_DEFAULT_INDEX, JOBSC_PAGE_DEFAULT_SIZE)
    {}

    /**
     * @brief 带参数构造函数
     * @param sort_field 排序字段
     * @param order_ype 排序类型
     * @param page_index 页码
     * @param page_size 每页大小
     */
    JobScDbPageQuery(const std::string& sort_field, JobScOrderType order_ype, uint32_t page_index, uint32_t page_size)
      : m_sort_field(sort_field)
      , m_order_type(order_ype)
      , m_page_index(page_index)
      , m_page_size(page_size)
    {
    }

    /**
     * @brief 获取排序字段
     * @return 排序字段
     */
    std::string GetSortField() const
    {
        return m_sort_field;
    }

    /**
     * @brief 获取排序字段C字符串
     * @return 排序字段C字符串
     */
    const char* GetSortFieldCStr() const
    {
        return m_sort_field.c_str();
    }

    /**
     * @brief 获取排序类型
     * @return 排序类型
     */
    JobScOrderType GetOrderType() const
    {
        return m_order_type;
    }

    /**
     * @brief 获取页码
     * @return 页码
     */
    uint32_t GetPageIndex() const
    {
        return m_page_index;
    }

    /**
     * @brief 获取每页大小
     * @return 每页大小
     */
    uint32_t GetPageSize() const
    {
        return m_page_size;
    }

    /**
     * @brief 设置排序字段
     * @param sortField 排序字段
     */
    void SetSortField(const std::string& sortField)
    {
        m_sort_field = sortField;
    }

    /**
     * @brief 设置排序类型
     * @param orderType 排序类型
     */
    void SetOrderType(JobScOrderType orderType)
    {
        m_order_type = orderType;
    }

    /**
     * @brief 设置页码
     * @param pageIndex 页码
     */
    void SetPageIndex(uint32_t pageIndex)
    {
        m_page_index = pageIndex;
    }

    /**
     * @brief 设置每页大小
     * @param pageSize 每页大小
     */
    void SetPageSize(uint32_t pageSize)
    {
        m_page_size = pageSize;
    }

  private:
    std::string m_sort_field;       ///< 排序字段
    JobScOrderType m_order_type;    ///< 排序类型
    uint32_t m_page_index;          ///< 页码
    uint32_t m_page_size;           ///< 每页大小
};

#endif  // JOBSCDBPAGEQUERY_H