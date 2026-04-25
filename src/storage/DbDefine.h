#ifndef DBDEFINE_H
#define DBDEFINE_H

#include <cstdint>

namespace shortcut {
// 排序类型
enum class DbOrderType : int32_t
{
    ASC = 0,
    DESC = 1
};

// 缓冲区大小常量
static constexpr uint32_t DB_SQL_BUF_SHORT = 64U;
static constexpr uint32_t DB_SQL_BUF_NORMAL = 512U;
static constexpr uint32_t DB_SQL_BUF_LONG = 1024U;

// 分页默认常量（无符号）
static constexpr uint32_t DB_PAGE_DEFAULT_INDEX = 0U;
static constexpr uint32_t DB_PAGE_DEFAULT_SIZE = 10U;

// 固定SQL常量
static constexpr char DB_SQL_PRAGMA_USER_VERSION[] = "PRAGMA user_version;";
static constexpr char DB_SQL_BEGIN_TRANSACTION[] = "BEGIN TRANSACTION;";
static constexpr char DB_SQL_COMMIT[] = "COMMIT;";
static constexpr char DB_SQL_ROLLBACK[] = "ROLLBACK;";
static constexpr char DB_SQL_INTEGRITY_CHECK[] = "PRAGMA integrity_check;";

static constexpr char DB_SQL_JOURNAL_MODE[] = "PRAGMA journal_mode = DELETE;";
static constexpr char DB_SQL_SYNCHRONOUS[] = "PRAGMA synchronous = FULL;";
static constexpr char DB_SQL_AUTO_VACUUM[] = "PRAGMA auto_vacuum = FULL;";
static constexpr char DB_SQL_TEMP_STORE[] = "PRAGMA temp_store = MEMORY;";
}  // namespace shortcut

#endif  // DBDEFINE_H