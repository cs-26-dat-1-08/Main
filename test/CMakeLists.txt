add_executable(test_osmlib test_osmlib.c)
target_link_libraries(test_osmlib mtest osmlib)
add_executable(test_algorithms test_algorithms.c)
target_link_libraries(test_algorithms mtest algorithms)

# Add test cases to CTest (our test framework has a nice macro for that).
discover_tests(test_osmlib test_algorithms)
