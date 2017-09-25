cmake_minimum_required(VERSION 3.6)
project(check_stats)

set(CMAKE_C_STANDARD 11)

set(SOURCE_FILES main.c)
add_executable(check_stats ${SOURCE_FILES})