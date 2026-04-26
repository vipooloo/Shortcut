#ifndef DB_DEFINE_H
#define DB_DEFINE_H

#include <cstdint>

namespace shortcut {
enum class DbOrderType : int32_t
{
    ASC = 0,
    DESC = 1
};

enum class DbValueType : int32_t
{
    Null = 0,
    Int32 = 1,
    Uint32 = 2,
    Int64 = 3,
    Uint64 = 4,
    String = 5,
    Blob = 6
};

static constexpr uint32_t DB_SQL_BUF_SHORT = 64U;
static constexpr uint32_t DB_SQL_BUF_NORMAL = 512U;
static constexpr uint32_t DB_SQL_BUF_LONG = 1024U;

static constexpr uint32_t DB_PAGE_DEFAULT_INDEX = 0U;
static constexpr uint32_t DB_PAGE_DEFAULT_SIZE = 10U;

static constexpr char DB_SQL_PRAGMA_USER_VERSION[] = "PRAGMA user_version;";
static constexpr char DB_SQL_BEGIN_TRANSACTION[] = "BEGIN TRANSACTION;";
static constexpr char DB_SQL_COMMIT[] = "COMMIT;";
static constexpr char DB_SQL_ROLLBACK[] = "ROLLBACK;";
static constexpr char DB_SQL_JOURNAL_MODE[] = "PRAGMA journal_mode = DELETE;";
static constexpr char DB_SQL_SYNCHRONOUS[] = "PRAGMA synchronous = FULL;";
static constexpr char DB_SQL_AUTO_VACUUM[] = "PRAGMA auto_vacuum = FULL;";
static constexpr char DB_SQL_TEMP_STORE[] = "PRAGMA temp_store = MEMORY;";
}  // namespace shortcut

#endif