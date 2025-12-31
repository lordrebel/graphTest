cmake_minimum_required(VERSION 3.10)
project(graphTest)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含头文件目录
include_directories(include)

# 添加测试
enable_testing()
add_subdirectory(src)
add_subdirectory(tests)