/**
 * @file JobScDbAccess.h
 * @brief 数据库访问层定义
 * @details 封装SQLite数据库操作，提供SQL执行、查询和事务管理功能
 */

#ifndef JOBSCDBACCESS_H
#define JOBSCDBACCESS_H

#include "JobScDbPageQuery.h"
#include "JobScTypes.h"
#include "JobScValue.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include <memory>

/**
 * @class JobScDbAccess
 * @brief 数据库访问类
 * @details 封装底层SQLite操作，提供数据库初始化、SQL执行、查询等功能
 */
class JobScDbAccess
{
  public:
    /**
     * @brief 默认构造函数
     */
    JobScDbAccess();
    ~JobScDbAccess() = default;

    /**
     * @brief 初始化数据库
     * @return 是否初始化成功
     */
    bool Init();
    /**
     * @brief 设置数据库路径
     * @param path 数据库文件路径
     */
    void SetDbPath(const std::string& path)
    {
        m_db_path = path;
    }
    /**
     * @brief 设置数据库版本
     * @param version 数据库版本号
     */
    void SetUserVersion(int32_t version)
    {
        m_user_version = version;
    }
    /**
     * @brief 设置升级回调
     * @param cb 升级回调函数
     */
    void SetUpgradeCallback(const UpgradeCallback& cb)
    {
        m_upgrade_callback = cb;
    }

    /**
     * @brief 执行SQL语句
     * @param sql SQL语句
     * @return 是否执行成功
     */
    bool ExecuteSql(const std::string& sql);
    /**
     * @brief 执行带参数的SQL语句
     * @param sql SQL语句
     * @param params 参数列表
     * @return 是否执行成功
     */
    bool ExecuteSql(const std::string& sql, const std::vector<JobScValue>& params);
    /**
     * @brief 获取最后插入行的ID
     * @return 最后插入行的ID
     */
    int64_t GetLastInsertRowId();

    /**
     * @brief 查询SQL语句
     * @param sql SQL语句
     * @param out_rows 输出行列表
     * @return 是否查询成功
     */
    bool QuerySql(const std::string& sql, JobScRowList& out_rows);
    /**
     * @brief 带参数查询SQL语句
     * @param sql SQL语句
     * @param params 参数列表
     * @param out_rows 输出行列表
     * @return 是否查询成功
     */
    bool QuerySql(const std::string& sql, const std::vector<JobScValue>& params, JobScRowList& out_rows);

    /**
     * @brief 开始事务
     */
    void BeginTransaction();
    /**
     * @brief 提交事务
     */
    void CommitTransaction();
    /**
     * @brief 回滚事务
     */
    void RollbackTransaction();

    /**
     * @brief 通用分页查询
     * @param sql_main 主SQL语句
     * @param params 参数列表
     * @param page_query 分页查询条件
     * @param out_result 输出分页结果
     * @param out_rows 输出行列表
     * @return 是否查询成功
     */
    bool QueryPageUniversal(const std::string& sql_main,
                            const std::vector<JobScValue>& params,
                            const JobScDbPageQuery& page_query,
                            JobScPageResult& out_result,
                            JobScRowList& out_rows);

  private:
    JobScDbAccess(const JobScDbAccess&) = delete;
    JobScDbAccess& operator=(const JobScDbAccess&) = delete;
    JobScDbAccess(JobScDbAccess&&) noexcept = delete;
    JobScDbAccess& operator=(JobScDbAccess&&) noexcept = delete;
    /**
     * @brief 获取数据库版本
     * @param out_version 输出版本号
     * @return 是否获取成功
     */
    bool GetDbVersion(int64_t& out_version);
    /**
     * @brief 设置数据库版本
     * @param version 版本号
     * @return 是否设置成功
     */
    bool SetDbVersion(int64_t version);
    /**
     * @brief 封装列值
     * @param col 数据库列
     * @return 封装后的值
     */
    static JobScValue WrapColumnValue(const SQLite::Column& col);
    /**
     * @brief 查询总记录数
     * @param sql_main 主SQL语句
     * @param params 参数列表
     * @param total 输出总记录数
     * @return 是否查询成功
     */
    bool QueryTotalCount(const std::string& sql_main,
                         const std::vector<JobScValue>& params,
                         uint32_t& total);
    /**
     * @brief 分页查询数据
     * @param sql_main 主SQL语句
     * @param params 参数列表
     * @param page_query 分页查询条件
     * @param total 总记录数
     * @param out_result 输出分页结果
     * @param out_rows 输出行列表
     * @return 是否查询成功
     */
    bool QueryPageData(const std::string& sql_main,
                       const std::vector<JobScValue>& params,
                       const JobScDbPageQuery& page_query,
                       uint32_t total,
                       JobScPageResult& out_result,
                       JobScRowList& out_rows);

  private:
    std::string m_db_path;                              ///< 数据库文件路径
    int64_t m_user_version;                             ///< 数据库版本号
    std::unique_ptr<SQLite::Database> m_db_ptr;         ///< SQLite数据库对象
    UpgradeCallback m_upgrade_callback;                 ///< 数据库升级回调函数
};

#endif  // JOBSCDBACCESS_H