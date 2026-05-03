#ifndef JOBSCSQLDEFINES_H
#define JOBSCSQLDEFINES_H

#include <cstdint>

static constexpr char DB_NAME[] = "shortcut.db";
static constexpr char DB_SQL_INIT[] = R"(
    CREATE TABLE IF NOT EXISTS shortcut (
        rid            INTEGER PRIMARY KEY,
        account_id     INTEGER,
        job_type       INTEGER,
        description    TEXT    NOT NULL,
        settings       TEXT,
        address_list   TEXT,
        create_time    INTEGER NOT NULL DEFAULT (strftime('%s', 'now')),  -- 插入自动赋值
        update_time    INTEGER NOT NULL DEFAULT (strftime('%s', 'now'))   -- 更新自动赋值
    );

    -- 触发器：每次更新时，自动刷新 update_time
    CREATE TRIGGER IF NOT EXISTS shortcut_update_time 
    AFTER UPDATE ON shortcut 
    FOR EACH ROW 
    BEGIN
        UPDATE shortcut SET update_time = strftime('%s', 'now') WHERE rid = OLD.rid;
    END;
)";

static constexpr uint32_t DB_SQL_BUFFER = 1024U;
static constexpr char DB_SQL_PRAGMA_USER_VERSION[] = "PRAGMA user_version;";
static constexpr char DB_SQL_BEGIN_TRANSACTION[] = "BEGIN TRANSACTION;";
static constexpr char DB_SQL_COMMIT_TRANSACTION[] = "COMMIT TRANSACTION;";
static constexpr char DB_SQL_ROLLBACK_TRANSACTION[] = "ROLLBACK TRANSACTION;";
static constexpr char DB_SQL_FOREIGN_KEYS[] = "PRAGMA foreign_keys = ON;";
static constexpr char DB_SQL_JOURNAL_MODE[] = "PRAGMA journal_mode = DELETE;";
static constexpr char DB_SQL_SYNCHRONOUS[] = "PRAGMA synchronous = FULL;";
static constexpr char DB_SQL_AUTO_VACUUM[] = "PRAGMA auto_vacuum = FULL;";
static constexpr char DB_SQL_TEMP_STORE[] = "PRAGMA temp_store = MEMORY;";

#endif  // JOBSCSQLDEFINES_H