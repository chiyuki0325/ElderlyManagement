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
ui.h
ui.c
menu.h
menu.c
members.h
members.c
utils.h
utils.c
save.h
save.c
poetry.h
poetry.c
house.h
house.c
bus.h
bus.c
venue.h
venue.c
servants.h
servants.c
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
    message(STATUS "Using MSVC")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fexec-charset=UTF-8 -finput-charset=UTF-8 -g -O0)
    message(STATUS "Using GNU")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})
