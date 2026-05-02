#include "../JobScLogger.h"
#include "../dao/JobScDao.h"
#include "JobScDbAccess.h"
#include "JobScDbValue.h"
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
    auto db_access = std::make_shared<JobScDbAccess>();
    db_access->SetDbPath("test.db");
    db_access->SetUserVersion(1);
    db_access->SetUpgradeCallback([&](int64_t old_ver, int64_t new_ver) -> bool {
        if (old_ver < 1)
        {
            if (!db_access->ExecuteSql(sql))
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
    if (db_access->Init())
    {
        std::cout << "db init success" << std::endl;
    }
    JobScDao dao(db_access);
    JobScRow entity;
    entity.emplace("job_type", static_cast<uint64_t>(1));
    entity.emplace("description", "abc");
    entity.emplace("settings", JobScDbValue(std::vector<uint8_t>{0x01, 0x02, 0x03}));
    entity.emplace("address_list", JobScDbValue(std::vector<uint8_t>{0x04, 0x05, 0x06}));
    int64_t id = 0;
    if (dao.Insert(123, entity, id))
    {
        std::cout << "insert success, id: " << id << std::endl;
    }
    JobScDbPageQuery page_query("rid", JobScOrderType::DESC, 0, 10);
    JobScPageResult out_result;
    JobScRowList out_list;
    dao.GetListByTypePage(
        "abc",
        static_cast<JobScType>(1),
        page_query,
        out_result,
        out_list);
    return 0;
}