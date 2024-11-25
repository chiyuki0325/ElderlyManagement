cmake_minimum_required(VERSION 3.10)
project(ElderlyManagement)

# 必须使用 C99 标准
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCES
main.c
)

add_executable(${PROJECT_NAME} ${SOURCES})