# 作业调度模块 (JobSc) 设计文档

## 1. 模块概述

### 1.1 模块简介

作业调度模块（JobSc - Job Scheduler）是快捷方式管理系统的核心组件，负责管理和维护用户的作业配置信息。该模块提供作业的增删改查功能，支持多种作业类型（如扫描到邮箱、FTP、SMB、USB等），并提供观察者模式用于事件通知。

### 1.2 功能特性

- **作业管理**：支持作业的添加、删除、修改、查询操作
- **分页查询**：支持按类型、关键字进行分页查询
- **观察者模式**：支持事件订阅和通知机制
- **数据库持久化**：使用SQLite进行数据存储
- **线程安全**：支持多线程并发访问
- **类型限制**：支持每种作业类型的数量限制

---

## 2. 系统架构

### 2.1 架构图

```
┌─────────────────────────────────────────────────────────────────────┐
│                         应用层 (JobScMgr)                           │
│                  静态接口，对外提供统一入口                          │
└─────────────────────────────────────────────────────────────────────┘
                                   │
                                   ▼
┌─────────────────────────────────────────────────────────────────────┐
│                      实现层 (JobScImpl)                             │
│               单例模式，业务逻辑处理，线程同步                       │
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────────────┐  │
│  │ JobScInitializer│  │  JobScDao     │  │ 观察者列表             │  │
│  │ 数据库初始化    │  │ 数据访问对象  │  │ m_observers           │  │
│  └────────────────┘  └────────────────┘  └────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────┘
                                   │
                                   ▼
┌─────────────────────────────────────────────────────────────────────┐
│                        数据访问层 (DAO)                            │
│                          JobScDao                                   │
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────────────┐  │
│  │ JobScTransGuard│  │ CRUD操作       │  │ 分页查询               │  │
│  │ 事务守卫       │  │ Insert/Delete  │  │ GetListByTypePage      │  │
│  └────────────────┘  │ Update         │  └────────────────────────┘  │
└───────────────────────────────────��─────────────────────────────────┘
                                   │
                                   ▼
┌─────────────────────────────────────────────────────────────────────┐
│                      数据库访问层 (DAL)                            │
│                      JobScDbAccess                                   │
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────────────┐  │
│  │ SQLite::Database│  │ SQL执行       │  │ 分页查询               │  │
│  │ 数据库实例     │  │ ExecuteSql    │  │ QueryPageUniversal     │  │
│  └────────────────┘  └────────────────┘  └────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────┘
                                   │
                                   ▼
┌─────────────────────────────────────────────────────────────────────┐
│                         SQLite 数据库                               │
│                        shortcut.db                                  │
│  ┌───────────────────────────────────────────────────────────────┐  │
│  │                      shortcut 表                              │  │
│  │  rid | account_id | job_type | description | settings | ...  │  │
│  └───────────────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────────────┘
```

### 2.2 层次结构

| 层次 | 职责 | 主要类 |
|------|------|--------|
| 应用层 | 对外提供静态接口 | JobScMgr |
| 实现层 | 业务逻辑处理，单例管理 | JobScImpl |
| DAO层 | 数据库操作封装 | JobScDao, JobScTransGuard |
| DAL层 | SQLite数据库操作 | JobScDbAccess |
| 实体层 | 数据模型 | JobScItem, JobScValue |

---

## 3. 模块结构

### 3.1 文件清单

```
src/
├── include/                          # 公共头文件目录
│   ├── JobScMgr.h                    # 管理器接口
│   ├── JobScItem.h                   # 作业项实体
│   ├── JobScPageQuery.h              # 分页查询条件
│   ├── JobScDefines.h                # 核心类型定义
│   └── JobScResult.h                 # 操作结果枚举
├── storage/                          # 存储层目录
│   ├── JobScDbAccess.h               # 数据库访问层
│   ├── JobScDbAccess.cpp
│   ├── JobScDbPageQuery.h           # 数据库分页查询
│   └── JobScDbPageQuery.cpp
├── dao/                              # 数据访问层目录
│   ├── JobScDao.h                   # 数据访问对象
│   ├── JobScDao.cpp
│   ├── JobScTransGuard.cpp          # 事务守卫
│   └── JobScTransGuard.h
├── JobScImpl.h                       # 实现类头文件
├── JobScImpl.cpp                     # 实现类实现
├── JobScInitializer.h                # 数据库初始化器
├── JobScInitializer.cpp
├── JobScValue.h                      # 数据库值封装
├── JobScValue.cpp
├── JobScTypes.h                      # 类型别名定义
├── JobScSqlDefines.h                  # SQL定义
└── JobScLogger.h                      # 日志宏定义

app/
├── main.cpp                          # 入口文件
├── JT_JobScTest.cpp                  # 单元测试
└── CMakeLists.txt                   # 构建配置
```

---

## 4. 核心类设计

### 4.1 JobScMgr - 作业调度管理器

**职责**：对外提供静态接口，是模块的入口点

**设计要点**：
- 所有方法为静态方法
- 内部委托给JobScImpl单例处理
- 线程安全

```cpp
class JobScMgr
{
public:
    // 添加作业
    static std::pair<JobScResult, JobScItem> Add(uint64_t account_id,
                                         JobScType type,
                                         const std::string& description,
                                         const std::vector<uint8_t>& job_settings,
                                         const std::vector<uint8_t>& addr_info);
    
    // 删除作业
    static JobScResult Delete(const std::vector<int64_t>& rids);
    
    // 按类型删除作业
    static JobScResult DeleteByType(const std::vector<JobScType>& types);
    
    // 更新作业
    static JobScResult Update(int64_t rid, const JobScItem& item);
    
    // 分页查询
    static JobScResult GetListByTypePage(const JobScPageQuery& page_query, 
                                       JobScPageResult& out_result, 
                                       std::vector<JobScItem>& out_items);
    
    // 观察者管理
    static void AddObserver(const JobScObserver& observer);
    static void RemoveObserver(const JobScObserver& observer);
};
```

### 4.2 JobScImpl - 作业调度实现类

**职责**：实现所有业务逻辑，负责线程同步和观察者通知

**设计要点**：
- 单例模式，保证全局唯一
- 使用互斥锁保证线程安全
- 维护观察者列表，事件发生时通知所有观察者

**成员变量**：
| 成员变量 | 类型 | 说明 |
|----------|------|------|
| m_service_mutex | std::mutex | 服务互斥锁，保护业务操作 |
| m_initializer | JobScInitializer | 数据库初始化器 |
| m_dao | JobScDao | 数据访问对象 |
| m_observers_mutex | std::mutex | 观察者列表互斥锁 |
| m_observers | std::list<JobScObserver> | 观察者列表 |

### 4.3 JobScItem - 作业项实体

**职责**：表示一个作业任务的数据模型

**属性**：
| 属性 | 类型 | 说明 |
|------|------|------|
| m_rid | int64_t | 主键ID |
| m_account_id | uint64_t | 账号ID |
| m_job_type | JobScType | 作业类型 |
| m_description | std::string | 描述 |
| m_settings | std::vector<uint8_t> | 作业设置 |
| m_address_list | std::vector<uint8_t> | 地址列表 |

### 4.4 JobScDao - 数据访问对象

**职责**：封装数据库CRUD操作

**主要方法**：
- Insert() - 插入记录
- Delete() / DeleteByType() - 删除记录
- Update() - 更新记录
- GetListByTypePage() - 分页查询
- GetCountByType() - 获取指定类型记录数

**内部方法**（私有）：
- BeginTransaction() / CommitTransaction() / RollbackTransaction() - 事务管理
- GetAllPage() - 查询全部数据
- GetAllByKeywordPage() - 关键字查询
- GetListByTypePage() - 按类型查询
- GetListByTypeAndKeywordPage() - 按类型和关键字查询

### 4.5 JobScTransGuard - 事务守卫

**职责**：RAII模式的事务管理，确保事务正确提交或回滚

**设计要点**：
- 构造时自动开启事务
- 析构时自动回滚未提交的事务
- ���动���用Commit()提交事务

### 4.6 JobScDbAccess - 数据库访问层

**职责**：封装SQLite数据库操作

**主要方法**：
| 方法 | 说明 |
|------|------|
| Init() | 初始化数据库连接 |
| ExecuteSql() | 执行SQL语句 |
| QuerySql() | 查询SQL语句 |
| BeginTransaction() | 开始事务 |
| CommitTransaction() | 提交事务 |
| RollbackTransaction() | 回滚事务 |
| QueryPageUniversal() | 通用分页查询 |

---

## 5. 核心类型定义

### 5.1 枚举类型

**JobScType - 作业类型**
```cpp
enum class JobScType : int32_t
{
    None = 0,       // 无类型
    ScanToEmail,    // 扫描到邮箱
    ScanToFTP,      // 扫描到FTP
    ScanToSMB,      // 扫描到SMB
    ScanToUSB,      // 扫描到USB
    All = 5         // 所有类型
};
```

**JobScOrderType - 排序类型**
```cpp
enum class JobScOrderType : int32_t
{
    ASC = 0,    // 升序
    DESC = 1    // 降序
};
```

**JobScEventType - 事件类型**
```cpp
enum class JobScEventType : int32_t
{
    Added,    // 新增事件
    Updated,  // 修改事件
    Deleted   // 删除事件
};
```

**JobScResult - 操作结果**
```cpp
enum class JobScResult : int32_t
{
    Success = 0,          // 成功
    Failed,               // 失败
    MaxCountReached,      // 超过最大限制
    NoExist,              // 记录不存在
    AlreadyExist,         // 已存在
    InvalidParam          // 参数错误
};
```

### 5.2 常量定义

| 常量 | 值 | 说明 |
|------|-----|------|
| JOBSC_PAGE_DEFAULT_INDEX | 0U | 默认分页起始页码 |
| JOBSC_PAGE_DEFAULT_SIZE | 10U | 默认分页每页大小 |
| MAX_COUNT_PER_TYPE | 10u | 每种类型最大作业数 |
| DB_NAME | "shortcut.db" | 数据库文件名 |

---

## 6. 数据库设计

### 6.1 表结构

```sql
CREATE TABLE IF NOT EXISTS shortcut (
    rid            INTEGER PRIMARY KEY,           -- 主键ID
    account_id     INTEGER,                       -- 账号ID
    job_type       INTEGER,                       -- 作业类型
    description    TEXT    NOT NULL,              -- 描述
    settings       TEXT,                          -- 作业设置
    address_list   TEXT,                          -- 地址列表
    create_time    INTEGER NOT NULL DEFAULT (strftime('%s', 'now')),  -- 创建时间
    update_time    INTEGER NOT NULL DEFAULT (strftime('%s', 'now'))   -- 更新时间
);
```

### 6.2 触发器

```sql
-- 自动更新触发器
CREATE TRIGGER IF NOT EXISTS shortcut_update_time 
AFTER UPDATE ON shortcut 
FOR EACH ROW 
BEGIN
    UPDATE shortcut SET update_time = strftime('%s', 'now') WHERE rid = OLD.rid;
END;
```

---

## 7. 业务流程

### 7.1 添加作业流程

```
JobScMgr::Add()
    │
    ▼
JobScImpl::Add()
    │
    ├──▶ 获取作业类型
    │
    ├──▶ 检查数量限制 (MAX_COUNT_PER_TYPE)
    │         │
    │         ├── 未超限 ──▶ 执行插入
    │         │
    │         └── 超限 ──▶ 返回 MaxCountReached
    │
    ├──▶ 构造行数据 JobScRow
    │
    ├──▶ 调用 m_dao.Insert()
    │         │
    │         └──▶ JobScDbAccess::ExecuteSql()
    │
    ├──▶ 成功 ──▶ Notify(JobScEventType::Added)
    │                  │
    │                  └──▶ 遍历观察者列表，调用回调函数
    │
    └──▶ 返回结果
```

### 7.2 查询作业流程

```
JobScMgr::GetListByTypePage()
    │
    ▼
JobScImpl::GetListByTypePage()
    │
    ├──▶ 调用 m_dao.GetListByTypePage()
    │         │
    │         └──▶ JobScDbAccess::QueryPageUniversal()
    │
    ├──▶ 调用 CovertrToItems() 转换数据
    │         │
    │         └──▶ 遍历 JobScRowList，构建 JobScItem 列表
    │
    └──▶ 返回结果
```

### 7.3 观察者模式

```
作业操作 (Add/Delete/Update)
    │
    ▼
触发 Notify()
    │
    ├──▶ 复制观察者列表 (加锁)
    │
    ├──▶ 遍历观察者列表
    │
    └──▶ 调用每个观察者的回调函数
              │
              └──▶ 传递 JobScEventType 事件
```

---

## 8. 线程安全设计

### 8.1 互斥锁使用

| 保护对象 | 互斥锁 | 说明 |
|----------|--------|------|
| 业务操作 | m_service_mutex | 保护Add/Delete/Update等业务方法 |
| 观察者列表 | m_observers_mutex | 保护观察者的添加/移除/通知 |

### 8.2 线程安全保证

1. **单例访问**：JobScImpl使用局部静态变量保证线程安全的单例
2. **锁粒度**：尽量减小锁的范围，提高并发度
3. **观察者复制**：Notify时复制观察者列表，避免长时间持有锁

---

## 9. 错误处理

### 9.1 错误类型

| 错误码 | 说明 | 常见场景 |
|--------|------|----------|
| Success | 成功 | 操作成功 |
| Failed | 失败 | 通用失败 |
| MaxCountReached | 超过最大限制 | 某类型作业数已达上限 |
| NoExist | 记录不存在 | 更新/删除不存在的记录 |
| AlreadyExist | 已存在 | 插入重复记录 |
| InvalidParam | 参数错误 | 非法参数 |

### 9.2 日志记录

使用统一的日志宏进行记录：
- `JOBSC_LOG_INFO` - 信息日志
- `JOBSC_LOG_WARN` - 警告日志
- `JOBSC_LOG_ERROR` - 错误日志
- `JOBSC_LOG_FORCE` - 强制日志（重要信息）

---

## 10. 使用示例

### 10.1 添加作业

```cpp
// 创建作业项
JobScItem item;
item.SetJobType(JobScType::ScanToEmail);
item.SetDescription("扫描到邮箱任务");
item.SetAccountId(123456789);

// 添加作业
auto result = JobScMgr::Add(123456789, item);

if (result.first == JobScResult::Success) {
    int64_t new_rid = result.second;
    std::cout << "作业添加成功, RID: " << new_rid << std::endl;
}
```

### 10.2 分页查询

```cpp
// 构建查询条件
JobScPageQuery query;
query.SetType(JobScType::ScanToEmail);  // 按类型查询
query.SetKeyword("关键字");              // 按关键字查询
query.SetPageIndex(0);                   // 第一页
query.SetPageSize(10);                   // 每页10条

// 执行查询
JobScPageResult page_result;
std::vector<JobScItem> items;
auto result = JobScMgr::GetListByTypePage(query, page_result, items);

if (result == JobScResult::Success) {
    std::cout << "总记录数: " << page_result.total_count << std::endl;
    std::cout << "总页数: " << page_result.total_page << std::endl;
    for (const auto& item : items) {
        std::cout << "RID: " << item.GetRid() 
                  << ", 描述: " << item.GetDescription() << std::endl;
    }
}
```

### 10.3 使用观察者

```cpp
// 创建观察者回调
auto observer = [](JobScEventType event) {
    switch (event) {
        case JobScEventType::Added:
            std::cout << "作业已添加" << std::endl;
            break;
        case JobScEventType::Updated:
            std::cout << "作业已更新" << std::endl;
            break;
        case JobScEventType::Deleted:
            std::cout << "作业已删除" << std::endl;
            break;
    }
};

// 注册观察者
JobScMgr::AddObserver(observer);

// 后续的作业操作都会触发回调通知
JobScMgr::Add(123456789, item);
```

---

## 11. 单元测试

### 11.1 测试文件

- `app/JT_JobScTest.cpp` - 使用Google Test框架的单元测试

### 11.2 测试套件

| 测试类 | 覆盖范围 |
|--------|----------|
| JobScItemTest | JobScItem构造函数、getter/setter、移动语义 |
| JobScPageQueryTest | 分页查询条件、枚举值、常量验证 |
| JobScObserverTest | 观察者模式添加/移除/多个观察者 |
| JobScMgrAddTest | JobScMgr::Add() - 各种作业类型、空参数、大数据 |
| JobScMgrDeleteTest | JobScMgr::Delete() / DeleteByType() |
| JobScMgrUpdateTest | JobScMgr::Update() |
| JobScMgrGetListTest | JobScMgr::GetListByTypePage() - 分页/关键字/排序 |
| JobScMgrIntegrationTest | 增删改查完整流程测试 |
| JobScMgrEdgeCaseTest | 边界情况测试 |
| JobScPageResultTest | 分页结果结构体测试 |

### 11.3 运行测试

```bash
# 构建项目
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j4

# 运行测试
./app/Myapp
```

---

## 12. 编译与运行

### 12.1 编译

```bash
mkdir build
cd build
cmake ..
make
```

### 12.2 运行

```bash
./app/Myapp
```

---

## 13. 设计原则

1. **单一职责**：每个类只负责一项功能
2. **依赖倒置**：高层模块不依赖低层模块，通过抽象接口交互
3. **最小知识**：只与直接相关的类交互
4. **开闭原则**：对扩展开放，对修改关闭
5. **RAII**：资源获取即初始化，使用智能指针和事务守卫管理资源

---

## 14. 依赖关系

### 14.1 第三方库

| 库名 | 版本 | 用途 |
|------|------|------|
| SQLiteCpp | latest | SQLite数据库封装 |
| googletest | latest | 单元测试框架 |

### 14.2 系统库

- pthread - 线程库
- dl - 动态链接库
- m - 数学库