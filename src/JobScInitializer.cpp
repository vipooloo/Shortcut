#include "JobScInitializer.h"
#include "JobScDbAccess.h"
#include "JobScLogger.h"
#include "JobScSqlDefines.h"

void JobScInitializer::Init(JobScDao& dao)
{
    std::shared_ptr<JobScDbAccess> db_access_ptr = std::make_shared<JobScDbAccess>();
    if (db_access_ptr)
    {
        db_access_ptr->SetDbPath(DB_NAME);
        db_access_ptr->SetUserVersion(1);
        db_access_ptr->SetUpgradeCallback(std::bind(&JobScInitializer::Upgrade, this, db_access_ptr, std::placeholders::_1, std::placeholders::_2));
        if (db_access_ptr->Init())
        {
            dao.SetDbAccess(db_access_ptr);
            JOBSC_LOG_FORCE("JobScInitializer::Init() - db init success");
        }
        else
        {
            JOBSC_LOG_ERROR("JobScInitializer::Init() - db init failed");
        }
    }
}

bool JobScInitializer::Upgrade(const std::shared_ptr<JobScDbAccess>& db_access_ptr, int64_t old_ver, int64_t new_ver)
{
    bool result{true};

    for (int64_t ver = old_ver + 1; ver <= new_ver; ++ver)
    {
        JOBSC_LOG_FORCE("JobScInitializer::Upgrade() - upgrade from %lld to %lld", old_ver, ver);
        if (ver == 1)
        {
            if (!db_access_ptr->ExecuteSql(DB_SQL_INIT))
            {
                result = false;
                JOBSC_LOG_ERROR("JobScInitializer::Upgrade() - execute sql failed for version %lld", ver);
                break;
            }
        }
    }

    return result;
}
