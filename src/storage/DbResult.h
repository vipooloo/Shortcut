#ifndef DB_RESULT_H
#define DB_RESULT_H

#include <cstdint>

namespace shortcut {
enum class DbCode : int32_t
{
    OK = 0,
    INTERNAL_ERROR,
    IO_ERROR,
    CORRUPTED,
    VERSION_ERR,
    INVALID_DATA,
};

class DbResult
{
  public:
    DbResult();
    explicit DbResult(DbCode code)
      : m_code{code}
    {
    }

    bool IsOk() const
    {
        return (DbCode::OK == m_code);
    }

    void SetCode(DbCode code)
    {
        m_code = code;
    }

    DbCode GetCode() const
    {
        return m_code;
    }

  private:
    DbCode m_code;
};
}  // namespace shortcut

#endif