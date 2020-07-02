// Copyright 2020 Yaroslav Sh.
#include <csv.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#define BACKTESTER_IMPL
// #include <backtester.hpp>
#include <strategies/weight_alloc.hpp>
#include <asset_alloc_lb.hpp>
#include <asset_alloc_rb.hpp>
#include <utils.hpp>
#include <single_asset.hpp>
#include <strategy.hpp>


#define REL_PATH
// #define TEST_REV_REL

int main() {
    bt::price_t pT {
        110.5, 113.1, 29.0, 220.4, 535.2, 58.5, 100.0, 200.1, 6.2};
    bt::weight_t wT {
          0.2,   0.8,  0.3,   0.4,   0.7,  0.2,   0.0,   0.9, 0.1};

    size_t n = 3, m = 2;
#ifdef REL_PATH
    auto strat = bt::strategy<bt::weight_alloc>(wT)
                     .lookback(m)
                     .rebalance_every(n);
#endif
#ifdef TEST_REV_REL
    auto strat = bt::strategy<bt::weight_alloc>(wT)
                     .rebalance_every(n)
                     .lookback(m);
#endif

    bt::single_asset back_test(strat);
    auto const &res = back_test.run(pT);
}
