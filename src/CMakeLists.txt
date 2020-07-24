add_library (BacktesterHpp 
    single_asset.cpp 
    utils.cpp 
    t_series.cpp 
    result.cpp
    mk_strategy.cpp)

target_include_directories (BacktesterHpp PUBLIC ../inc)

