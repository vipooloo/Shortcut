#include "JobScValue.h"
#include <algorithm>
#include <cstring>

JobScValue::JobScValue()
  : m_type{JobScValType::Null}
  , m_data{}
{
}

JobScValue::JobScValue(const JobScValue& other)
{
    m_type = other.m_type;
    m_data = other.m_data;
}

JobScValue::JobScValue(JobScValue&& other) noexcept
{
    if (this != &other)
    {
        m_type = other.m_type;
        m_data = std::move(other.m_data);
    }
}

JobScValue& JobScValue::operator=(const JobScValue& other)
{
    if (this != &other)
    {
        m_type = other.m_type;
        m_data = other.m_data;
    }
    return *this;
}

JobScValue& JobScValue::operator=(JobScValue&& other) noexcept
{
    if (this != &other)
    {
        m_type = other.m_type;
        m_data = std::move(other.m_data);
    }
    return *this;
}

JobScValue::JobScValue(int64_t value)
  : m_type{JobScValType::Int}
  , m_data{}
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
    m_data.assign(p, p + sizeof(int64_t));
}

JobScValue::JobScValue(uint64_t value)
  : m_type{JobScValType::Int}
  , m_data{}
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
    m_data.assign(p, p + sizeof(int64_t));
}

JobScValue::JobScValue(const std::string& value)
  : m_type{JobScValType::String}
  , m_data{}
{
    m_data.assign(value.cbegin(), value.cend());
}

JobScValue::JobScValue(const std::vector<uint8_t>& value)
  : m_type{JobScValType::Blob}
  , m_data{value.cbegin(), value.cend()}
{
}

JobScValue::JobScValue(const uint8_t* data, uint32_t len)
  : m_type{JobScValType::Blob}
  , m_data{}
{
    if ((data != nullptr) && (len > 0U))
    {
        m_data.assign(data, data + len);
    }
}

int32_t JobScValue::ToInt32(bool& ok) const
{
    int64_t val{0};
    ok = false;

    if (m_type == JobScValType::Int)
    {
        std::memcpy(&val, m_data.data(), std::min(m_data.size(), sizeof(int64_t)));
        ok = true;
    }
    return static_cast<int32_t>(val);
}

uint32_t JobScValue::ToUint32(bool& ok) const
{
    int64_t val{0};
    ok = false;

    if (m_type == JobScValType::Int)
    {
        std::memcpy(&val, m_data.data(), std::min(m_data.size(), sizeof(int64_t)));
        ok = true;
    }
    return static_cast<uint32_t>(val);
}

uint64_t JobScValue::ToUint64(bool& ok) const
{
    int64_t val{0};
    ok = false;

    if (m_type == JobScValType::Int)
    {
        std::memcpy(&val, m_data.data(), std::min(m_data.size(), sizeof(int64_t)));
        ok = true;
    }
    return static_cast<uint64_t>(val);
}

int64_t JobScValue::ToInt64(bool& ok) const
{
    int64_t val{0};
    ok = false;

    if (m_type == JobScValType::Int)
    {
        std::memcpy(&val, m_data.data(), std::min(m_data.size(), sizeof(int64_t)));
        ok = true;
    }
    return val;
}

std::string JobScValue::ToString(bool& ok) const
{
    std::string res{""};
    ok = false;

    if (JobScValType::String == m_type)
    {
        res.assign(m_data.begin(), m_data.end());
        ok = true;
    }
    return res;
}

const std::vector<uint8_t>& JobScValue::ToBlob() const
{
    return m_data;
}

JobScValue::operator int32_t() const
{
    bool result{false};
    return ToInt32(result);
}

JobScValue::operator uint32_t() const
{
    bool result{false};
    return ToUint32(result);
}

JobScValue::operator int64_t() const
{
    bool result{false};
    return ToInt64(result);
}

JobScValue::operator uint64_t() const
{
    bool result{false};
    return ToUint64(result);
}

JobScValue::operator std::string() const
{
    bool result{false};
    return ToString(result);
}

JobScValue::operator std::vector<uint8_t>() const
{
    return ToBlob();
}

JobScValue::operator const std::vector<uint8_t>&() const
{
    return ToBlob();
}
