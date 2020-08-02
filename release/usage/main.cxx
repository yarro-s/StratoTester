/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

// #include <csv.h>
// #include <unistd.h>
#include <iostream>
#include <vector>
#define STRATOTESTER_IMPL
#include <latest/StratoTester.hpp>

int main() {
    st::price const init_cash = 10000;
    st::prices pT{150.5, 75.2, 125.3, 36.8,  305,  145,  8.5};
    st::weights wT{      0.24, 0.26,  0.82, 0.73, 0.61, 0.44};

    st::weight_alloc strat(wT);
    st::single_asset back_test(strat, init_cash);

    auto res = back_test.run(pT).results();

    std::cout << std::endl
                << " >>> PV(T) = " << st::str_rep(res.value_history())
                << std::endl;
    std::cout << std::endl
                << " >>> W(T) = " << st::str_rep(res.asset_weights())
                << std::endl;
}

/*
int main() {
    // auto const init_deposit = 10000;
    // st::prices pT {
    //     110.5, 113.1, 29.0, 220.4, 535.2, 58.5, 100.0, 200.1, 6.2};
    // // st::weights wT {
    // //       0.2,   0.8,  0.3,   0.4,   0.7,  0.2,   0.1,   0.9, 0.1};
    // auto const w = 0.5;

    // st::buy_and_hold strat(w);
    // st::single_asset back_test(strat, init_deposit);
    // auto res = back_test.run(pT).results();

    // std::cout << std::endl
    //             << st::str_rep(res.value_history()) << std::endl;
}
*/