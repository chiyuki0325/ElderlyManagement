# 项目采用 CMake 构建系统
# 使用 Code::Blocks 内建的构建功能可以直接编译运行
# 亦可使用 VSCode、JetBrains CLion 等 IDE，或直接使用命令行构建

cmake_minimum_required(VERSION 3.10)
project(ElderlyManagement)

# 必须使用 C99 标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCES
main.c
ui.c
ui.h
menu.c
menu.h
)

add_executable(${PROJECT_NAME} ${SOURCES})