cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# 生成可执行文件（自动包含所有找到的源码）
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

# 获取所有子目录
file(GLOB_RECURSE _SUB_DIRS LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/*)

# 只添加目录
foreach(_DIR ${_SUB_DIRS})
    if(IS_DIRECTORY ${_DIR})
        include_directories(${_DIR})
    endif()
endforeach()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 添加链接库
target_link_libraries(${PROJECT_NAME} SQLiteCpp)
target_link_libraries(${PROJECT_NAME} sqlite3)
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} dl)
target_link_libraries(${PROJECT_NAME} m)

add_compile_options(
        -Wall               # 基础警告
        -Wextra             # 额外警告
        -Wpedantic          # 严格遵循标准
        -Werror             # 所有警告=错误
        -Wconversion        # 类型隐式转换警告
        -Wsign-conversion   # 有符号无符号转换警告
        -Wuninitialized     # 未初始化变量
        -Wshadow            # 变量遮蔽
        -Wnon-virtual-dtor  # 非虚析构函数（C++ 重要）
)