#include "DbAccess.h"

int main(int argc, char* argv[])
{
    shortcut::DbAccess db_access;
    db_access.SetDbPath("test.db");
    db_access.SetUpgradeCallback([](int64_t old_ver, int64_t new_ver) -> bool {
        // 在这里执行数据库升级逻辑，例如根据版本号执行不同的 SQL 语句
        // 你可以使用 db_access.ExecuteSql(...) 来执行升级 SQL
        // 返回 true 表示升级成功，false 表示升级失败
        return true;
    });
    db_access.SetUserVersion(1);
    if (db_access.Init())
    {
        // Database initialized successfully, you can perform database operations here
    }
    return 0;
}