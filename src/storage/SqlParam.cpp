#include "SqlParam.h"
#include <cstring>

namespace shortcut {

SqlParam::SqlParam(int32_t value)
  : m_type(Type::kInt32)
  , m_bin_data()
{
    m_bin_data.resize(sizeof(int32_t));
    std::memcpy(m_bin_data.data(), &value, sizeof(int32_t));
}

SqlParam::SqlParam(int64_t value)
  : m_type(Type::kInt64)
  , m_bin_data()
{
    m_bin_data.resize(sizeof(int64_t));
    std::memcpy(m_bin_data.data(), &value, sizeof(int64_t));
}

SqlParam::SqlParam(const std::string& value)
  : m_type(Type::kString)
  , m_bin_data()
{
    m_bin_data.assign(value.begin(), value.end());
}

SqlParam::SqlParam(const SqlParam& other)
  : m_type(other.m_type)
  , m_bin_data(other.m_bin_data)
{
}

SqlParam& SqlParam::operator=(const SqlParam& other)
{
    if (this != &other)
    {
        m_type = other.m_type;
        m_bin_data = other.m_bin_data;
    }
    return *this;
}

SqlParam::SqlParam(SqlParam&& other) noexcept
  : m_type(other.m_type)
  , m_bin_data(std::move(other.m_bin_data))
{
}

SqlParam& SqlParam::operator=(SqlParam&& other) noexcept
{
    if (this != &other)
    {
        m_type = other.m_type;
        m_bin_data = std::move(other.m_bin_data);
    }
    return *this;
}

}  // namespace shortcut