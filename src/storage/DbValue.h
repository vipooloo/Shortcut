#ifndef DB_VALUE_H
#define DB_VALUE_H

#include "DbDefine.h"
#include <cstdint>
#include <string>
#include <vector>

namespace shortcut {
class DbValue
{
  public:
    DbValue();
    explicit DbValue(const std::string& value);
    DbValue(const uint8_t* data, uint32_t len);
    DbValue(const DbValue& other);
    DbValue(DbValue&& other) noexcept;
    DbValue& operator=(const DbValue& other);
    DbValue& operator=(DbValue&& other) noexcept;

    explicit DbValue(int64_t value);
    ~DbValue() = default;

    DbValueType GetType() const
    {
        return m_type;
    }

    bool IsNull() const
    {
        return (DbValueType::Null == m_type);
    }

    int32_t ToInt32(bool& ok) const;
    uint32_t ToUint32(bool& ok) const;
    int64_t ToInt64(bool& ok) const;
    uint64_t ToUint64(bool& ok) const;
    std::string ToString(bool& ok) const;
    const std::vector<uint8_t>& ToBlob() const;

    operator int32_t() const;
    operator uint32_t() const;
    operator int64_t() const;
    operator uint64_t() const;
    operator std::string() const;
    operator std::vector<uint8_t>() const;
    operator const std::vector<uint8_t>&() const;

  private:
    DbValueType m_type;
    std::vector<uint8_t> m_data;
};
}  // namespace shortcut

#endif