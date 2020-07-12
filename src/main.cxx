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
          0.2,   0.8,  0.3,   0.4,   0.7,  0.2,   0.1,   0.9, 0.1};

    size_t n_lb = 3, m_rb = 4;

    // auto *w_alloc = new bt::weight_alloc(wT);
    // auto strat_lb = new bt::lookback(w_alloc, n_lb);
    // auto strat_rb = new bt::rebalance(w_alloc, m_rb);

    // auto strat = w_alloc;
    // auto strat = strat_lb->rebalance_every(m_rb);
    // auto strat = strat_rb->set_lookback(n_lb);

    bt::weight_alloc strat(wT);
    bt::single_asset back_test(strat, 10000);
    auto t = back_test.run(pT);

    std::cout << std::endl
              << bt::str_rep(t.results().pv()) << std::endl;
}
