#include "DbAccess.h"
#include "ShortcutLogger.h"

int main(int argc, char* argv[])
{
    shortcut::DbAccess db_access;
    db_access.SetDbPath("test.db");
    db_access.SetUserVersion(1);
    db_access.SetUpgradeCallback([&](int64_t old_ver, int64_t new_ver) -> bool {
        if (old_ver < 1)
        {
            // 第一次建库
            const char* sql = "CREATE TABLE IF NOT EXISTS test_table (...)";
            if (!db_access.ExecuteSql(sql))
                return false;
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