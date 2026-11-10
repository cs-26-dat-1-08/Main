# Setting this policy to NEW avoids a warning in newer versions of CMake.
cmake_policy(SET CMP0135 NEW)

# Installation of a testing framework - here the simple mtest.
include(FetchContent)
FetchContent_Declare(geodc
        URL https://github.com/geographiclib/geographiclib-c/archive/refs/tags/v2.2.zip
        URL_HASH SHA256=ffe91434410df000a02f7e5f3842b0c84623122123ef055058f62d4ddccbaf68
)
FetchContent_MakeAvailable(geodc)
