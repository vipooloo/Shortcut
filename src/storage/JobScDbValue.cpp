#include "JobScDbValue.h"
#include <algorithm>
#include <cstring>

JobScDbValue::JobScDbValue()
  : m_type{JobScDbValType::Null}
  , m_data{}
{
}

JobScDbValue::JobScDbValue(const JobScDbValue& other)
{
    m_type = other.m_type;
    m_data = other.m_data;
}

JobScDbValue::JobScDbValue(JobScDbValue&& other) noexcept
{
    if (this != &other)
    {
        m_type = other.m_type;
        m_data = std::move(other.m_data);
    }
}

JobScDbValue& JobScDbValue::operator=(const JobScDbValue& other)
{
    if (this != &other)
    {
        m_type = other.m_type;
        m_data = other.m_data;
    }
    return *this;
}

JobScDbValue& JobScDbValue::operator=(JobScDbValue&& other) noexcept
{
    if (this != &other)
    {
        m_type = other.m_type;
        m_data = std::move(other.m_data);
    }
    return *this;
}

JobScDbValue::JobScDbValue(int64_t value)
  : m_type{JobScDbValType::Int}
  , m_data{}
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
    m_data.assign(p, p + sizeof(int64_t));
}

JobScDbValue::JobScDbValue(uint64_t value)
  : m_type{JobScDbValType::Int}
  , m_data{}
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
    m_data.assign(p, p + sizeof(int64_t));
}

JobScDbValue::JobScDbValue(const std::string& value)
  : m_type{JobScDbValType::String}
  , m_data{}
{
    m_data.assign(value.cbegin(), value.cend());
}

JobScDbValue::JobScDbValue(const std::vector<uint8_t>& value)
  : m_type{JobScDbValType::Blob}
  , m_data{value.cbegin(), value.cend()}
{
}

JobScDbValue::JobScDbValue(const uint8_t* data, uint32_t len)
  : m_type{JobScDbValType::Blob}
  , m_data{}
{
    if ((data != nullptr) && (len > 0U))
    {
        m_data.assign(data, data + len);
    }
}

int32_t JobScDbValue::ToInt32(bool& ok) const
{
    int64_t val{0};
    ok = false;

    if (m_type == JobScDbValType::Int)
    {
        std::memcpy(&val, m_data.data(), std::min(m_data.size(), sizeof(int64_t)));
        ok = true;
    }
    return static_cast<int32_t>(val);
}

uint32_t JobScDbValue::ToUint32(bool& ok) const
{
    int64_t val{0};
    ok = false;

    if (m_type == JobScDbValType::Int)
    {
        std::memcpy(&val, m_data.data(), std::min(m_data.size(), sizeof(int64_t)));
        ok = true;
    }
    return static_cast<uint32_t>(val);
}

uint64_t JobScDbValue::ToUint64(bool& ok) const
{
    int64_t val{0};
    ok = false;

    if (m_type == JobScDbValType::Int)
    {
        std::memcpy(&val, m_data.data(), std::min(m_data.size(), sizeof(int64_t)));
        ok = true;
    }
    return static_cast<uint64_t>(val);
}

int64_t JobScDbValue::ToInt64(bool& ok) const
{
    int64_t val{0};
    ok = false;

    if (m_type == JobScDbValType::Int)
    {
        std::memcpy(&val, m_data.data(), std::min(m_data.size(), sizeof(int64_t)));
        ok = true;
    }
    return val;
}

std::string JobScDbValue::ToString(bool& ok) const
{
    std::string res{""};
    ok = false;

    if (JobScDbValType::String == m_type)
    {
        res.assign(m_data.begin(), m_data.end());
        ok = true;
    }
    return res;
}

const std::vector<uint8_t>& JobScDbValue::ToBlob() const
{
    return m_data;
}

JobScDbValue::operator int32_t() const
{
    bool result{false};
    return ToInt32(result);
}

JobScDbValue::operator uint32_t() const
{
    bool result{false};
    return ToUint32(result);
}

JobScDbValue::operator int64_t() const
{
    bool result{false};
    return ToInt64(result);
}

JobScDbValue::operator uint64_t() const
{
    bool result{false};
    return ToUint64(result);
}

JobScDbValue::operator std::string() const
{
    bool result{false};
    return ToString(result);
}

JobScDbValue::operator std::vector<uint8_t>() const
{
    return ToBlob();
}

JobScDbValue::operator const std::vector<uint8_t>&() const
{
    return ToBlob();
}
