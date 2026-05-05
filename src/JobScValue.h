/**
 * @file JobScValue.h
 * @brief 数据库值封装类
 * @details 定义用于封装数据库字段值的类型，支持多种数据类型
 */

#ifndef JOBSCVALUE_H
#define JOBSCVALUE_H

#include "JobScDefines.h"
#include <cstdint>
#include <string>
#include <vector>

/**
 * @enum JobScValType
 * @brief 值类型枚举
 */
enum class JobScValType : int32_t
{
    Null = 0,
    Int,
    String,
    Blob,
};

/**
 * @class JobScValue
 * @brief 数据库字段值封装类
 * @details 用于封装不同类型的数据库字段值，支持Null、Int、String、Blob类型
 */
class JobScValue
{
  public:
    /**
     * @brief 默认构造函数
     */
    JobScValue();

    /**
     * @brief 整型构造函数
     * @param value 整数值
     */
    explicit JobScValue(int64_t value);

    /**
     * @brief 无符号整型构造函数
     * @param value 无符号整数值
     */
    explicit JobScValue(uint64_t value);

    /**
     * @brief 字符串构造函数
     * @param value 字符串值
     */
    explicit JobScValue(const std::string& value);

    /**
     * @brief Blob构造函数
     * @param value 二进制数据
     */
    JobScValue(const std::vector<uint8_t>& value);

    /**
     * @brief Blob构造函数
     * @param data 数据指针
     * @param len 数据长度
     */
    JobScValue(const uint8_t* data, uint32_t len);

    /**
     * @brief 拷贝构造函数
     * @param other 源对象
     */
    JobScValue(const JobScValue& other);

    /**
     * @brief 移动构造函数
     * @param other 源对象
     */
    JobScValue(JobScValue&& other) noexcept;

    /**
     * @brief 拷贝赋值运算符
     * @param other 源对象
     * @return 自身引用
     */
    JobScValue& operator=(const JobScValue& other);

    /**
     * @brief 移动赋值运算符
     * @param other 源对象
     * @return 自身引用
     */
    JobScValue& operator=(JobScValue&& other) noexcept;

    ~JobScValue() = default;

    /**
     * @brief 获取值类型
     * @return 值类型
     */
    JobScValType GetType() const
    {
        return m_type;
    }

    /**
     * @brief 判断是否为空
     * @return 是否为空
     */
    bool IsNull() const
    {
        return (JobScValType::Null == m_type);
    }

    /**
     * @brief 转换为32位整数
     * @param ok 输出参数，是否转换成功
     * @return 32位整数
     */
    int32_t ToInt32(bool& ok) const;

    /**
     * @brief 转换为32位无符号整数
     * @param ok 输出参数，是否转换成功
     * @return 32位无符号整数
     */
    uint32_t ToUint32(bool& ok) const;

    /**
     * @brief 转换为64位整数
     * @param ok 输出参数，是否转换成功
     * @return 64位整数
     */
    int64_t ToInt64(bool& ok) const;

    /**
     * @brief 转换为64位无符号整数
     * @param ok 输出参数，是否转换成功
     * @return 64位无符号整数
     */
    uint64_t ToUint64(bool& ok) const;

    /**
     * @brief 转换为字符串
     * @param ok 输出参数，是否转换成功
     * @return 字符串
     */
    std::string ToString(bool& ok) const;

    /**
     * @brief 转换为二进制数据
     * @return 二进制数据
     */
    const std::vector<uint8_t>& ToBlob() const;

    operator int32_t() const;
    operator uint32_t() const;
    operator int64_t() const;
    operator uint64_t() const;
    operator std::string() const;
    operator const std::vector<uint8_t>&() const;

  private:
    JobScValType m_type;          ///< 值类型
    std::vector<uint8_t> m_data;  ///< 存储的数据
};

#endif  // JOBSCVALUE_H