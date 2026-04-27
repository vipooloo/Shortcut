#include "DbAccess.h"

int main(int argc, char* argv[])
{
    shortcut::DbAccess db_access;
    db_access.SetDbPath("test.db");
    db_access.SetUserVersion(1);
    if (db_access.Init())
    {
        // Database initialized successfully, you can perform database operations here
    }
    return 0;
}