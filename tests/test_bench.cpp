/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <iostream>
#include <catch2/catch.hpp>

#include <buy_and_hold.hpp>
#include <single_asset.hpp>

TEST_CASE("Basic usage showcases", "[usage]") {
    bt::price_t price_hist {110.5, 113.1, 29.0, 220.4, 565.3, 42.0};

    SECTION("backtester.hpp is built correctly") {
        auto const init_deposit = 1000;
        auto const w = 0.5;

        bt::buy_and_hold strat(w);
        bt::single_asset back_test(strat, init_deposit);
        auto res = back_test.run(price_hist).results();

        auto const res_expected = "[1000, 663.6, 1429.2, 2808.8, 715.6]";

        REQUIRE(res_expected == bt::str_rep(res.pv()));
    }
}