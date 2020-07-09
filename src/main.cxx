// Copyright 2020 Yaroslav Sh.

#include <csv.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#define BACKTESTER_IMPL
// #include <backtester.hpp>
#include <strategies/weight_alloc.hpp>
#include <utils.hpp>
#include <single_asset.hpp>
#include <strategy.hpp>
#include <rebalance.hpp>
#include <lookback.hpp>


int main() {
    bt::price_t pT {
        110.5, 113.1, 29.0, 220.4, 535.2, 58.5, 100.0, 200.1, 6.2};
    bt::weight_t wT {
          0.2,   0.8,  0.3,   0.4,   0.7,  0.2,   0.0,   0.9, 0.1};

    size_t n = 3, m = 2;

    auto w_alloc = bt::weight_alloc(wT);
    auto strat = bt::lookback(&w_alloc, 3);


    bt::single_asset back_test(strat);
    auto t = back_test.run(pT);

    std::cout << std::endl << t.results().growth() << std::endl;
}
