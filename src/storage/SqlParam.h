#ifndef SQL_PARAM_H
#define SQL_PARAM_H

#include <cstdint>
#include <string>
#include <vector>

namespace shortcut {

class SqlParam
{
  public:
    explicit SqlParam(int32_t value);
    explicit SqlParam(int64_t value);
    explicit SqlParam(const std::string& value);

    SqlParam(const SqlParam& other);
    SqlParam& operator=(const SqlParam& other);

    SqlParam(SqlParam&& other) noexcept;
    SqlParam& operator=(SqlParam&& other) noexcept;

    ~SqlParam() = default;

    enum class Type
    {
        kInt32,
        kInt64,
        kString
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

}  // namespace shortcut

#endif