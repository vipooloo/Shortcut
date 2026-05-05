/**
 * @file JobScSqlDefines.h
 * @brief 数据库SQL定义
 * @details 定义数据库表结构、字段名称和SQL语句常量
 */

#ifndef JOBSCSQLDEFINES_H
#define JOBSCSQLDEFINES_H

#include <cstdint>

/**
 * @brief 数据库文件名
 */
static constexpr char DB_NAME[] = "shortcut.db";
/**
 * @brief 数据库初始化SQL脚本
 */
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

static constexpr char DB_SQL_PRAGMA_USER_VERSION[] = "PRAGMA user_version;";    ///< 获取数据库版本
static constexpr char DB_SQL_BEGIN_TRANSACTION[] = "BEGIN TRANSACTION;";        ///< 开始事务
static constexpr char DB_SQL_COMMIT_TRANSACTION[] = "COMMIT TRANSACTION;";      ///< 提交事务
static constexpr char DB_SQL_ROLLBACK_TRANSACTION[] = "ROLLBACK TRANSACTION;";  ///< 回滚事务
static constexpr char DB_SQL_FOREIGN_KEYS[] = "PRAGMA foreign_keys = ON;";      ///< 启用外键约束
static constexpr char DB_SQL_JOURNAL_MODE[] = "PRAGMA journal_mode = DELETE;";  ///< 日志模式
static constexpr char DB_SQL_SYNCHRONOUS[] = "PRAGMA synchronous = FULL;";      ///< 同步模式
static constexpr char DB_SQL_AUTO_VACUUM[] = "PRAGMA auto_vacuum = FULL;";      ///< 自动清理
static constexpr char DB_SQL_TEMP_STORE[] = "PRAGMA temp_store = MEMORY;";      ///< 临时表存储

/**
 * @brief 数据库表名
 */
constexpr char kTableName[] = "shortcut";
/**
 * @brief 字段名: 主键ID
 */
constexpr char kFieldRid[] = "rid";
/**
 * @brief 字段名: 账号ID
 */
constexpr char kFieldAccountId[] = "account_id";
/**
 * @brief 字段名: 作业类型
 */
constexpr char kFieldJobType[] = "job_type";
/**
 * @brief 字段名: 描述
 */
constexpr char kFieldDescription[] = "description";
/**
 * @brief 字段名: 设置
 */
constexpr char kFieldSettings[] = "settings";
/**
 * @brief 字段名: 地址列表
 */
constexpr char kFieldAddressList[] = "address_list";

static constexpr uint32_t DB_SQL_BUFFER = 1024U;     ///< SQL缓冲区大小
static constexpr uint64_t MAX_COUNT_PER_TYPE = 10u;  ///< 每种类型最大数
static constexpr int32_t JOBSCDB_VERSION = 1;        ///< 数据库版本

#endif  // JOBSCSQLDEFINES_H