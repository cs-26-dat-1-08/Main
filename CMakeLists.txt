cmake_minimum_required(VERSION 3.14)
project(Main C)
set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include dependencies
find_package(LibXml2 REQUIRED)
include_directories({LIBXML2_INCLUDE_DIR})

include(cmake/mtest.cmake)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)
