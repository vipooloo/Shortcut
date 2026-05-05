/**
 * @file JobScPageQuery.h
 * @brief 分页查询条件定义
 * @details 定义作业调度系统中的分页查询参数类
 */

#ifndef JOBSCPAGEQUERY_H
#define JOBSCPAGEQUERY_H

#include "JobScDefines.h"
#include <cstdint>
#include <string>

/**
 * @class JobScPageQuery
 * @brief 分页查询条件类
 * @details 用于封装分页查询的各种条件参数
 */
class JobScPageQuery
{
  public:
    JobScPageQuery()
      : m_keyword{""}
      , m_type{JobScType::All}
      , m_order_type{JobScOrderType::ASC}
      , m_page_index{JOBSC_PAGE_DEFAULT_INDEX}
      , m_page_size{JOBSC_PAGE_DEFAULT_SIZE}
    {}

    ~JobScPageQuery() = default;

    /**
     * @brief 获取关键字
     * @return 关键字
     */
    const std::string& GetKeyword() const
    {
        return m_keyword;
    }

    /**
     * @brief 设置关键字
     * @param keyword 关键字
     */
    void SetKeyword(const std::string& keyword)
    {
        m_keyword = keyword;
    }

    /**
     * @brief 获取作业类型
     * @return 作业类型
     */
    JobScType GetType() const
    {
        return m_type;
    }

    /**
     * @brief 设置作业类型
     * @param type 作业类型
     */
    void SetType(JobScType type)
    {
        m_type = type;
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
     * @brief 设置排序类型
     * @param order_type 排序类型
     */
    void SetOrderType(JobScOrderType order_type)
    {
        m_order_type = order_type;
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
     * @brief 设置页码
     * @param page_index 页码
     */
    void SetPageIndex(uint32_t page_index)
    {
        m_page_index = page_index;
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
     * @brief 设置每页大小
     * @param page_size 每页大小
     */
    void SetPageSize(uint32_t page_size)
    {
        m_page_size = page_size;
    }

  private:
    std::string m_keyword;        ///< 关键字
    JobScType m_type;             ///< 作业类型
    JobScOrderType m_order_type;  ///< 排序类型
    uint32_t m_page_index;        ///< 页码
    uint32_t m_page_size;         ///< 每页大小
};

#endif  // JOBSCPAGEQUERY_H