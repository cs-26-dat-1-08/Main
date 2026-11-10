cmake_minimum_required(VERSION 3.14)
project(Main C)
set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Make another CMAKE_BUILD_TYPE called CUSTOMDEBUG
set(CMAKE_C_FLAGS_CUSTOMDEBUG "-Wall -pedantic -g -fsanitize=address" CACHE STRING
  "Flags used by the C compiler during customdebug builds."
  FORCE)
set(CMAKE_EXE_LINKER_FLAGS_CUSTOMDEBUG
  "-Wl,--warn-unresolved-symbols,--warn-once -g -fsanitize=address" CACHE STRING
  "Flags used for linking binaries during customdebug builds."
  FORCE)
set(CMAKE_SHARED_LINKER_FLAGS_CUSTOMDEBUG
  "-Wl,--warn-unresolved-symbols,--warn-once -g -fsanitize=address" CACHE STRING
  "Flags used by the shared libraries linker during customdebug builds."
  FORCE)
mark_as_advanced(
  CMAKE_C_FLAGS_CUSTOMDEBUG
  CMAKE_EXE_LINKER_FLAGS_CUSTOMDEBUG
  CMAKE_SHARED_LINKER_FLAGS_CUSTOMDEBUG)
set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
  "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel CustomDebug."
  FORCE)

# Include dependencies
find_package(LibXml2 REQUIRED)
include_directories({LIBXML2_INCLUDE_DIR})
include(cmake/mtest.cmake)
include(cmake/geographiclib-c.cmake)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)
