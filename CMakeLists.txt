cmake_minimum_required(VERSION 3.14)
project(Main C)
set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include dependencies
include(cmake/mtest.cmake)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)
