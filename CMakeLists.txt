cmake_minimum_required(VERSION 3.10)

# set the project name
project(BTLight VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(src/BTLightConfig.h.in src/BTLightConfig.h)

# add the executable
add_executable(BTLight src/hello.cpp)

target_include_directories(BTLight PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
