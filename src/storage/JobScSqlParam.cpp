#include "JobScSqlParam.h"
#include <cstring>

JobScSqlParam::JobScSqlParam(int64_t value)
  : m_type(Type::kInt)
  , m_bin_data()
{
    m_bin_data.resize(sizeof(int64_t));
    std::memcpy(m_bin_data.data(), &value, sizeof(int64_t));
}

JobScSqlParam::JobScSqlParam(const std::string& value)
  : m_type(Type::kString)
  , m_bin_data()
{
    m_bin_data.assign(value.begin(), value.end());
}

JobScSqlParam::JobScSqlParam(const std::vector<uint8_t>& value)
  : m_type(Type::kBinary)
  , m_bin_data(value)
{
}

JobScSqlParam::JobScSqlParam(const JobScSqlParam& other)
  : m_type(other.m_type)
  , m_bin_data(other.m_bin_data)
{
}

JobScSqlParam& JobScSqlParam::operator=(const JobScSqlParam& other)
{
    if (this != &other)
    {
        m_type = other.m_type;
        m_bin_data = other.m_bin_data;
    }
    return *this;
}

JobScSqlParam::JobScSqlParam(JobScSqlParam&& other) noexcept
  : m_type(other.m_type)
  , m_bin_data(std::move(other.m_bin_data))
{
}

JobScSqlParam& JobScSqlParam::operator=(JobScSqlParam&& other) noexcept
{
    if (this != &other)
    {
        m_type = other.m_type;
        m_bin_data = std::move(other.m_bin_data);
    }
    return *this;
}
