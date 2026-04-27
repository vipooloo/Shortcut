cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# 生成可执行文件（自动包含所有找到的源码）
add_executable(Myapp ${SRC_FILES})

# 添加当前目录为头文件搜索路径
target_include_directories(Myapp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Myapp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/include)
target_include_directories(Myapp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../src/storage)
# 添加链接库
target_link_libraries(Myapp ShortcutMgr)

