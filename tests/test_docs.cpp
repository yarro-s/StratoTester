/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <csv.h>

#include <iostream>
#include <catch2/catch.hpp>

#include <latest/StratoTester.hpp>

st::t_series<st::price> read_from_csv(
    std::string const &datapath,
    std::string const &dataset_name,
    std::string const &index_name,
    std::string const &field_name,
    std::string const &time_fmt = "%Y-%m-%d");


TEST_CASE("Docs tests", "[usage]") {
    st::prices price_hist {110.5, 113.1, 29.0, 220.4, 565.3, 42.0};

    SECTION("1") {
        auto const init_deposit = 1000;
        auto const w = 0.5;

        st::const_alloc strat(w);
        st::single_asset back_test(strat, init_deposit);
        auto res = back_test.run(price_hist).results();

        auto const res_expected = "[1000, 663.6, 1429.2, 2808.8, 715.6]";

        // REQUIRE(res_expected == st::str_rep(res.pv()));
    }
}
