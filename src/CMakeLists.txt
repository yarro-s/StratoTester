add_library (BacktesterHpp 
    single_asset.cpp 
    utils.cpp 
    t_series.cpp 
    result.cpp)

target_include_directories (BacktesterHpp PUBLIC ../inc)

