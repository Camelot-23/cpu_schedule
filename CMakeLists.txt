cmake_minimum_required(VERSION 3.26)
project(cpu_schedule C)

set(CMAKE_C_STANDARD 11)

# 源文件列表
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS src/*.c)

add_executable(cpu_schedule ${srcs})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)
