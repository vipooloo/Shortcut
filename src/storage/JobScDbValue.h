#ifndef DBVALUE_H
#define DBVALUE_H

#include "JobScDefines.h"
#include <cstdint>
#include <string>
#include <vector>

enum class JobScDbValType : int32_t
{
    Null = 0,
    Int,
    String,
    Blob,
};

class JobScDbValue
{
  public:
    JobScDbValue();
    explicit JobScDbValue(const std::string& value);
    JobScDbValue(const uint8_t* data, uint32_t len);
    JobScDbValue(const JobScDbValue& other);
    JobScDbValue(JobScDbValue&& other) noexcept;
    JobScDbValue& operator=(const JobScDbValue& other);
    JobScDbValue& operator=(JobScDbValue&& other) noexcept;

    explicit JobScDbValue(int64_t value);
    ~JobScDbValue() = default;

    JobScDbValType GetType() const
    {
        return m_type;
    }

    bool IsNull() const
    {
        return (JobScDbValType::Null == m_type);
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
    JobScDbValType m_type;
    std::vector<uint8_t> m_data;
};

#endif  // DBVALUE_H