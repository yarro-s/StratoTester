cmake_minimum_required (VERSION 3.10)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED True)

project (BTLight VERSION 1.0)

add_subdirectory (src) 
add_subdirectory (tests)

enable_testing ()
add_test (NAME MyTest COMMAND Test)

