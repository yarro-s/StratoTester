add_executable (TestAll 
    test_main.cpp
    test_weight_alloc.cpp
    test_balance_book.cpp
    test_single_asset.cpp
    test_result.cpp
    test_lookback.cpp
    test_rebalance.cpp
    test_rebalanced_lookback.cpp
    test_t_series.cpp)

target_include_directories (TestAll PUBLIC ../inc)
target_include_directories (TestAll PUBLIC ./lib)

target_link_libraries (TestAll BacktesterHpp)

add_executable (TestReal 
    test_main.cpp
    test_usage.cpp)

target_include_directories (TestReal PUBLIC ../release)
target_include_directories (TestReal PUBLIC ./lib)
