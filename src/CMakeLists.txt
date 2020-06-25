add_executable (BTLightMain main.mm)
add_library (BTLight 
    single_asset.cpp 
    utils.cpp 
    t_series.cpp 
    result.cpp)

target_include_directories (BTLightMain PUBLIC ../lib)
target_include_directories (BTLightMain PUBLIC ../inc)
target_include_directories (BTLight PUBLIC ../inc)
target_include_directories (BTLightMain PUBLIC ..)

target_link_libraries (BTLightMain BTLight)
