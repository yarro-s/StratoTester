/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <csv.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#define BACKTESTER_IMPL
#include <latest/backtester.hpp>


int main() {
    auto const init_deposit = 10000;
    bt::price_t pT {
        110.5, 113.1, 29.0, 220.4, 535.2, 58.5, 100.0, 200.1, 6.2};
    // bt::weight_t wT {
    //       0.2,   0.8,  0.3,   0.4,   0.7,  0.2,   0.1,   0.9, 0.1};
    auto const w = 0.5;

    bt::buy_and_hold strat(w);
    bt::single_asset back_test(strat, init_deposit);
    auto res = back_test.run(pT).results();

    std::cout << std::endl
                << bt::str_rep(res.pv()) << std::endl;
}
