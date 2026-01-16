cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)
set(PROJECT_NAME "macrochip")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "debug")

project(${PROJECT_NAME})

add_subdirectory(dependencies)
add_subdirectory(lib)

add_executable("macrochip"
    src/main.c
)

target_include_directories("macrochip" PRIVATE include)
target_link_libraries("macrochip" PRIVATE "macrochip-lib")

