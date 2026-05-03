#ifndef JOBSCVALUE_H
#define JOBSCVALUE_H

#include "JobScDefines.h"
#include <cstdint>
#include <string>
#include <vector>

enum class JobScValType : int32_t
{
    Null = 0,
    Int,
    String,
    Blob,
};

class JobScValue
{
  public:
    JobScValue();
    explicit JobScValue(int64_t value);
    explicit JobScValue(uint64_t value);
    explicit JobScValue(const std::string& value);
    JobScValue(const std::vector<uint8_t>& value);
    JobScValue(const uint8_t* data, uint32_t len);
    JobScValue(const JobScValue& other);
    JobScValue(JobScValue&& other) noexcept;
    JobScValue& operator=(const JobScValue& other);
    JobScValue& operator=(JobScValue&& other) noexcept;

    ~JobScValue() = default;

    JobScValType GetType() const
    {
        return m_type;
    }

    bool IsNull() const
    {
        return (JobScValType::Null == m_type);
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
    JobScValType m_type;
    std::vector<uint8_t> m_data;
};

#endif  // JOBSCVALUE_H