cmake_minimum_required (VERSION 3.10)

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED True)

SET(GCC_COMPILE_FLAGS "-pedantic -Wall -Wextra")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")

project (BacktesterHpp VERSION 0.1)

add_subdirectory (src) 
add_subdirectory (tests)

enable_testing ()
add_test (NAME DevTest COMMAND TestAll)
add_test (NAME UsageTest COMMAND TestReal)
