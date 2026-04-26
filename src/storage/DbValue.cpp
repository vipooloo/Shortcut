#include "DbValue.h"
#include <algorithm>
#include <cstring>

namespace shortcut {
DbValue::DbValue()
  : m_type{DbValueType::Null}
  , m_data{}
{
}

DbValue::DbValue(int32_t value)
  : m_type{DbValueType::Int32}
  , m_data{}
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
    m_data.assign(p, p + sizeof(int32_t));
}

DbValue::DbValue(uint32_t value)
  : m_type{DbValueType::Uint32}
  , m_data{}
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
    m_data.assign(p, p + sizeof(uint32_t));
}

DbValue::DbValue(int64_t value)
  : m_type{DbValueType::Int64}
  , m_data{}
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
    m_data.assign(p, p + sizeof(int64_t));
}

DbValue::DbValue(uint64_t value)
  : m_type{DbValueType::Uint64}
  , m_data{}
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
    m_data.assign(p, p + sizeof(uint64_t));
}

DbValue::DbValue(const std::string& value)
  : m_type{DbValueType::String}
  , m_data{}
{
    m_data.assign(value.begin(), value.end());
}

DbValue::DbValue(const uint8_t* data, uint32_t len)
  : m_type{DbValueType::Blob}
  , m_data{}
{
    if ((data != nullptr) && (len > 0U))
    {
        m_data.assign(data, data + len);
    }
}

int32_t DbValue::ToInt32(bool& ok) const
{
    int32_t val{0};
    ok = false;

    if ((DbValueType::Int32 == m_type) && (sizeof(int32_t) == m_data.size()))
    {
        std::memcpy(&val, m_data.data(), sizeof(int32_t));
        ok = true;
    }
    return val;
}

uint32_t DbValue::ToUint32(bool& ok) const
{
    uint32_t val{0U};
    ok = false;

    if ((DbValueType::Uint32 == m_type) && (sizeof(uint32_t) == m_data.size()))
    {
        std::memcpy(&val, m_data.data(), sizeof(uint32_t));
        ok = true;
    }
    return val;
}

int64_t DbValue::ToInt64(bool& ok) const
{
    int64_t val{0};
    ok = false;

    if ((DbValueType::Int64 == m_type) && (sizeof(int64_t) == m_data.size()))
    {
        std::memcpy(&val, m_data.data(), sizeof(int64_t));
        ok = true;
    }
    return val;
}

uint64_t DbValue::ToUint64(bool& ok) const
{
    uint64_t val{0U};
    ok = false;

    if ((DbValueType::Uint64 == m_type) && (sizeof(uint64_t) == m_data.size()))
    {
        std::memcpy(&val, m_data.data(), sizeof(uint64_t));
        ok = true;
    }
    return val;
}

std::string DbValue::ToString(bool& ok) const
{
    std::string res{""};
    ok = false;

    if (DbValueType::String == m_type)
    {
        res.assign(m_data.begin(), m_data.end());
        ok = true;
    }
    return res;
}

const std::vector<uint8_t>& DbValue::ToBlob() const
{
    return m_data;
}

DbValue::operator int32_t() const
{
    bool result{false};
    int32_t val = ToInt32(result);
    return val;
}

DbValue::operator uint32_t() const
{
    bool result{false};
    uint32_t val = ToUint32(result);
    return val;
}

DbValue::operator int64_t() const
{
    bool result{false};
    int64_t val = ToInt64(result);
    return val;
}

DbValue::operator uint64_t() const
{
    bool result{false};
    uint64_t val = ToUint64(result);
    return val;
}

DbValue::operator std::string() const
{
    bool result{false};
    std::string val = ToString(result);
    return val;
}

DbValue::operator std::vector<uint8_t>() const
{
    std::vector<uint8_t> val = ToBlob();
    return val;
}

DbValue::operator const std::vector<uint8_t>&() const
{
    return ToBlob();
}

}  // namespace shortcut