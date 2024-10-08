# 指定CMake的最低版本
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(myexpr)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 包含头文件路径
include_directories(include)

# 查找所有源文件
file(GLOB SOURCES "src/*.cpp" "main.cpp")

# 生成可执行文件
add_executable(myexpr ${SOURCES})

# 可选：设置编译选项
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

