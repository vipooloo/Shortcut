/**
 * @file JobScDao.h
 * @brief 数据访问对象定义
 * @details 负责数据库操作，封装CRUD等底层数据库访问逻辑
 */

#ifndef JOBSCDAO_H
#define JOBSCDAO_H

#include "JobScTypes.h"
#include <memory>

class JobScDbAccess;
class JobScDbPageQuery;

/**
 * @class JobScDao
 * @brief 数据访问对象类
 * @details 封装数据库操作，提供增删改查等接口
 */
class JobScDao
{
  public:
    /**
     * @class JobScTransGuard
     * @brief 事务守卫类
     * @details RAII模式的事务管理，确保事务正确提交或回滚
     */
    class JobScTransGuard
    {
      public:
        /**
         * @brief 构造函数
         * @param dao 数据访问对象引用
         */
        explicit JobScTransGuard(JobScDao& dao);
        /**
         * @brief 析构函数，自动回滚未提交的事务
         */
        ~JobScTransGuard();
        /**
         * @brief 提交事务
         */
        void Commit();

      private:
        JobScTransGuard(const JobScTransGuard&) = delete;
        JobScTransGuard(JobScTransGuard&&) noexcept = delete;
        JobScTransGuard& operator=(const JobScTransGuard&) = delete;
        JobScTransGuard& operator=(JobScTransGuard&&) noexcept = delete;

      private:
        JobScDao& m_dao;   ///< 数据访问对象引用
        bool m_committed;  ///< 事务是否已提交
    };

  public:
    /**
     * @brief 默认构造函数
     */
    JobScDao();
    /**
     * @brief 带参数的构造函数
     * @param db_ptr 数据库访问指针
     */
    explicit JobScDao(const std::shared_ptr<JobScDbAccess>& db_ptr);
    ~JobScDao() = default;
    /**
     * @brief 设置数据库访问对象
     * @param db_ptr 数据库访问指针
     */
    void SetDbAccess(const std::shared_ptr<JobScDbAccess>& db_ptr)
    {
        m_db_ptr = db_ptr;
    }
    /**
     * @brief 插入记录
     * @param account_id 账号ID
     * @param row_data 行数据
     * @param out_id 输出插入的记录ID
     * @return 是否成功
     */
    bool Insert(uint64_t account_id, const JobScRow& row_data, int64_t& out_id);
    /**
     * @brief 删除记录
     * @param rid 记录ID
     * @return 是否成功
     */
    bool Delete(int64_t rid);
    /**
     * @brief 按类型删除记录
     * @param type 作业类型
     * @return 是否成功
     */
    bool DeleteByType(JobScType type);
    /**
     * @brief 更新记录
     * @param rid 记录ID
     * @param row_data 行数据
     * @return 是否成功
     */
    bool Update(int64_t rid, const JobScRow& row_data);

    /**
     * @brief 分页查询作业列表
     * @param keyword 关键字
     * @param type 作业类型
     * @param page_query 分页查询条件
     * @param out_result 输出分页结果
     * @param out_list 输出记录列表
     * @return 是否成功
     */
    bool Query(
        const std::string& keyword,
        JobScType type,
        const JobScDbPageQuery& page_query,
        JobScPageResult& out_result,
        JobScRowList& out_list);
    /**
     * @brief 获取指定类型的记录数量
     * @param account_id 账号ID
     * @param job_type 作业类型
     * @return 记录数量
     */
    uint64_t GetCountByType(uint64_t account_id, JobScType job_type);
    /**
     * @brief 检查必填字段
     * @param row_data 行数据
     * @param required_fields 必填字段列表
     * @return 是否所有必填字段都存在
     */
    bool CheckRequiredFields(
        const JobScRow& row_data,
        const std::vector<std::string>& required_fields);

  private:
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
     * @brief 获取全部分页数据
     * @param page_query 分页查询条件
     * @param out_result 输出分页结果
     * @param out_list 输出记录列表
     * @return 是否成功
     */
    bool GetAllPage(const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);
    /**
     * @brief 关键字查询分页数据
     * @param keyword 关键字
     * @param page_query 分页查询条件
     * @param out_result 输出分页结果
     * @param out_list 输出记录列表
     * @return 是否成功
     */
    bool GetAllByKeywordPage(const std::string& keyword, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);
    /**
     * @brief 按类型查询分页数据
     * @param type 作业类型
     * @param page_query 分页查询条件
     * @param out_result 输出分页结果
     * @param out_list 输出记录列表
     * @return 是否成功
     */
    bool Query(JobScType type, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);
    /**
     * @brief 按类型和关键字查询分页数据
     * @param type 作业类型
     * @param keyword 关键字
     * @param page_query 分页查询条件
     * @param out_result 输出分页结果
     * @param out_list 输出记录列表
     * @return 是否成功
     */
    bool GetListByTypeAndKeywordPage(JobScType type, const std::string& keyword, const JobScDbPageQuery& page_query, JobScPageResult& out_result, JobScRowList& out_list);

  private:
    std::shared_ptr<JobScDbAccess> m_db_ptr;
};

#endif  // JOBSCDAO_H