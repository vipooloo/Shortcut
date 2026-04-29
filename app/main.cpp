#include "JobScDbAccess.h"
#include "JobScLogger.h"
#include <iostream>

static constexpr const char* sql = R"(
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

int main(int argc, char* argv[])
{
    JobScDbAccess db_access;
    db_access.SetDbPath("test.db");
    db_access.SetUserVersion(1);
    db_access.SetUpgradeCallback([&](int64_t old_ver, int64_t new_ver) -> bool {
        if (old_ver < 1)
        {
            if (!db_access.ExecuteSql(sql))
            {
                std::cout << "failed to create tables" << std::endl;
                return false;
            }
        }

        if (old_ver < 2)
        {
        }

        if (old_ver < 3)
        {
        }
        return true;
    });
    if (db_access.Init())
    {
    }
    return 0;
}