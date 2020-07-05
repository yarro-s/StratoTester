add_executable (Test 
    test_main.cpp
    test_usage.cpp
    test_weight_alloc.cpp
    test_balance_book.cpp)

target_include_directories (Test PUBLIC ../inc)
target_include_directories (Test PUBLIC ../lib)

target_link_libraries (Test BTLight)
