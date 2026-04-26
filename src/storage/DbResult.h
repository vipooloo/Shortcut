#ifndef DB_RESULT_H
#define DB_RESULT_H

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
    DbResult();
    void SetCode(DbCode code);
    bool IsOk() const;
    DbCode GetCode() const;

  private:
    DbCode m_code;
};
}  // namespace shortcut

#endif