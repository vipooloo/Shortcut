#ifndef JOBSCSQLPARAM_H
#define JOBSCSQLPARAM_H

#include <cstdint>
#include <string>
#include <vector>

class JobScSqlParam
{
  public:
    explicit JobScSqlParam(int64_t value);
    explicit JobScSqlParam(const std::string& value);
    explicit JobScSqlParam(const std::vector<uint8_t>& value);

    JobScSqlParam(const JobScSqlParam& other);
    JobScSqlParam& operator=(const JobScSqlParam& other);

    JobScSqlParam(JobScSqlParam&& other) noexcept;
    JobScSqlParam& operator=(JobScSqlParam&& other) noexcept;

    ~JobScSqlParam() = default;

    enum class Type
    {
        kInt,
        kString,
        kBinary,
    };

    Type GetType() const
    {
        return m_type;
    }

    const std::vector<uint8_t>& GetData() const
    {
        return m_bin_data;
    }

  private:
    Type m_type;
    std::vector<uint8_t> m_bin_data;
};

#endif // JOBSCSQLPARAM_H