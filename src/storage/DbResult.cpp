#include "DbResult.h"

namespace shortcut {
DbResult::DbResult()
  : m_code(DbCode::OK)
{
}

void DbResult::SetCode(DbCode code)
{
    m_code = code;
}

bool DbResult::IsOk() const
{
    return (DbCode::OK == m_code);
}

DbCode DbResult::GetCode() const
{
    return m_code;
}
}  // namespace shortcut