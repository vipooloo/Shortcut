#ifndef DBRESULT_H
#define DBRESULT_H

#include <cstdint>

namespace shortcut {
enum class DbCode : int32_t
{
    OK = 0,
    NOT_INIT = 1,
    INVALID_ARG = 2,
    INVALID_SQL = 3,
    IO_ERROR = 4,
    CORRUPTED = 5,
    VERSION_ERR = 6,
    INVALID_DATA = 7
};

class DbResult
{
  public:
    DbResult() = default;

    void SetCode(DbCode code)
    {
        m_code = code;
    }

    bool IsOk() const
    {
        return DbCode::OK == m_code;
    }

    DbCode GetCode() const
    {
        return m_code;
    }

  private:
    DbCode m_code{DbCode::OK};
};
}  // namespace shortcut

#endif  // DBRESULT_H