#include "DbAccess.h"
#include "ShortcutLogger.h"

int main(int argc, char* argv[])
{
    shortcut::DbAccess db_access;
    db_access.SetDbPath("test.db");
    db_access.SetUserVersion(1);
    db_access.SetUpgradeCallback([&](int64_t old_ver, int64_t new_ver) -> bool {
        if (old_ver == 0)
        {
            // create table
            const char* sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT)";
            if (!db_access.ExecuteSql(sql))
            {
                LOG_ERROR("Failed to create table: %s", sql);
                return false;
            }
        }
        return true;
    });
    if (db_access.Init())
    {
    }
    return 0;
}