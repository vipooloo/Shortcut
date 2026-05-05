# CMake 最低版本
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(ShortcutMgr)

# 设置 C++ 标准为 C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 如果没有指定构建类型，则默认设置为 Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()

# ===================== 新增：ASAN 与覆盖率选项 =====================
# 可通过 cmake 命令行开启：-DENABLE_ASAN=ON  -DENABLE_COVERAGE=ON
option(ENABLE_ASAN "Enable AddressSanitizer" ON)
option(ENABLE_COVERAGE "Enable code coverage" ON)

# ASAN 编译选项（仅Debug + GCC/Clang）
if(ENABLE_ASAN AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "AddressSanitizer enabled")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
endif()

# 代码覆盖率选项（仅Debug + GCC/Clang）
if(ENABLE_COVERAGE AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Code coverage enabled")
    add_compile_options(-fprofile-arcs -ftest-coverage)
    add_link_options(-fprofile-arcs -ftest-coverage)
endif()
# ==================================================================

option(SQLITECPP_RUN_CPPCHECK OFF)
# 你的第三方库根目录
set(SQLITE_ROOT 
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/SQLiteCpp/include 
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/SQLiteCpp/sqlite3
)

# 递归查找该目录下 所有子文件夹（包含所有层级）
file(GLOB_RECURSE ALL_INCLUDE_DIRS ${SQLITE_ROOT}/*)

# 过滤出所有目录（只保留文件夹路径）
foreach(dir ${ALL_INCLUDE_DIRS})
    if(IS_DIRECTORY ${dir})
        list(APPEND SYSTEM_INCLUDE_DIRS ${dir})
    endif()
endforeach()
# 把所有找到的目录都添加到头文件搜索路径
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include)

add_subdirectory(src)
add_subdirectory(third_party/SQLiteCpp)
add_subdirectory(third_party/googletest)
add_subdirectory(app)