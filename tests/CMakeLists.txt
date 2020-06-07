cmake_minimum_required(VERSION 3.10)

# set the project name
project(BTLight VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(src/BTLightConfig.h.in src/BTLightConfig.h)

# add the test
add_subdirectory(libs/Catch2)
target_link_libraries(tests Catch2::Catch2)

target_include_directories(BTLight PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
