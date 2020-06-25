add_executable (Test 
    test_main.cpp 
    test_utils.cpp 
    single_asset_test.cpp 
    t_series_test.cpp 
    test_result.cpp 
    asset_alloc_test.cpp 
    integration_test.cpp 
    balance_book_test.cpp)

target_include_directories (Test PUBLIC ../inc)
target_include_directories (Test PUBLIC ../lib)

target_link_libraries (Test BTLight)
