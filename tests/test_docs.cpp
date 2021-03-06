/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <csv.h>

#include <iostream>
#include <catch2/catch.hpp>

#include <StratoTester.hpp>  // #include "StratoTester.hpp"

using namespace st;

TEST_CASE("Docs tests", "[usage]") {
    // QQQ monthly quotes from Jan 01, 2019 to Dec 01, 2019
    auto qqq_hist = st::prices
        {168.16, 173.19, 179.66, 189.54, 173.95, 186.74,
         191.10, 187.47, 188.81, 197.08, 205.10, 212.61};

    SECTION("Simple momentum indicator") {
        auto last_close_up = [&](prices const &price_hist) {
            auto const last_close = price_hist.back();
            auto const first_close = price_hist.front();

            // buy when last close is higer than the first, hold otherwise
            return last_close > first_close ? 1.0 : 0.0;
        };

        auto strat = strategy::with(last_close_up)
                         .look_back(3)         // 3 samples rolling window
                         .rebalance_every(2);  // rebanace every 2 samples

        auto init_deposit = 10000;
        auto test = st::single_asset(strat, init_deposit).run(qqq_hist);

        std::cout << std::endl
                  << "Single asset portfolio value history: " << std::endl
                  << "   " << str_rep(test.results().value_history())
                  << std::endl;
        std::cout << std::endl
                  << "Single asset portfolio total return: "
                  << test.results().total_return() << std::endl;
    }
}


st::t_series<st::price> read_from_csv(
    std::string const &datapath,
    std::string const &dataset_name,
    std::string const &index_name,
    std::string const &field_name,
    std::string const &time_fmt = "%Y-%m-%d");
