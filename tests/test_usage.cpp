/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <csv.h>

#include <iostream>
#include <catch2/catch.hpp>


#define STRATOTESTER_IMPL
#include <latest/StratoTester.hpp>

st::t_series<st::price> read_from_csv(
    std::string const &datapath,
    std::string const &dataset_name,
    std::string const &index_name,
    std::string const &field_name,
    std::string const &time_fmt = "%Y-%m-%d") {


    st::t_series<st::price> ts;

    io::CSVReader<2, io::trim_chars<>>
        in(datapath + dataset_name + ".csv");

    in.read_header(io::ignore_extra_column,
        index_name, field_name);

    std::string date;
    st::price px;

    while (in.read_row(date, px)) {
        auto const t_stamp = st::str_to_time(date, time_fmt);

        ts.append_at(dataset_name, t_stamp, px);
    }

    return ts;
}


TEST_CASE("Real-world data showcase", "[usage]") {
    auto const init_deposit = 1000000;

    std::string const datapath = "../tests/data/";

    std::string const dataset_name("^IXIC");
    st::t_series<st::price> ts(read_from_csv("../tests/data/",
        dataset_name, "Date", "Close", "%Y-%m-%d"));

    SECTION("Buy-and-hold strategy") {
        st::const_alloc buy_and_hold(0.99);
        st::single_asset test(buy_and_hold, init_deposit);

        auto const test_res =
            test.run(ts.vals_for(dataset_name)).results();

        std::cout << std::endl
                  << "TOTAL RET: " << test_res.growth() << std::endl;
    }
}


TEST_CASE("Basic usage showcases", "[usage]") {
    st::prices price_hist {110.5, 113.1, 29.0, 220.4, 565.3, 42.0};

    SECTION("backtester.hpp is built correctly") {
        auto const init_deposit = 1000;
        auto const w = 0.5;

        st::const_alloc strat(w);
        st::single_asset back_test(strat, init_deposit);
        auto res = back_test.run(price_hist).results();

        auto const res_expected = "[1000, 663.6, 1429.2, 2808.8, 715.6]";

        REQUIRE(res_expected == st::str_rep(res.pv()));
    }
}
