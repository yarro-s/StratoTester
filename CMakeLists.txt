cmake_minimum_required(VERSION 3.10)

# set the project name
project(BTLight VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Main project files
add_executable(BTLight src/main.cpp src/single_asset.cpp)
target_include_directories(BTLight PUBLIC inc)
