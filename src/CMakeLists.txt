add_library (StratoTester 
    single_asset.cpp 
    utils.cpp 
    t_series.cpp 
    result.cpp)

target_include_directories (StratoTester PUBLIC ../inc)

