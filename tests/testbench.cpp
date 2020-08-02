/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <csv.h>

#include <ctime>
#include <iostream>
#include <numeric>
#include <iomanip>
#include <chrono>

#include <catch2/catch.hpp>

#include <strategy.hpp>
#include <single_asset.hpp>
#include <allocators/lambda_alloc.hpp>
#include <allocators/const_alloc.hpp>
#include <allocators/weight_alloc.hpp>
#include <utils.hpp>
#include <t_series.hpp>



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



TEST_CASE("Real data", "[usage]") {
    std::string const datapath = "../tests/data/";

    std::string const dataset_name("^IXIC");

    st::t_series<st::price> ts(read_from_csv("../tests/data/",
            dataset_name, "Date", "Close", "%Y-%m-%d"));

    // st::price_t price_hist {
    //     116.5, 13.1, 9.2, 520.4, 12.2, 88.5, 100.0, 800.1, 3.2};

    SECTION("MA cross NDX") {
        size_t n_long = 5;
        size_t m_short = 3;

        double const alpha_long = 1.0 / (1.0 + n_long),
                     alpha_short = 1.0 / (1.0 + m_short);

        double ema_long = 0.0, ema_short = 0.0;

        auto const trading_rule = [&](st::prices const &hist) {
            ema_long = hist.back() * alpha_long
                + ema_long * (1 - alpha_long);
            ema_short = hist.back() * alpha_short
                + ema_short * (1 - alpha_short);

            auto const signal = ema_short > ema_long ? 1.0 : 0.0;
            return signal;
        };

        st::lambda_alloc ma_cross(trading_rule);
        auto strat = st::strategy(&ma_cross)
            .look_back(n_long)
            .rebalance_every(30);

        st::single_asset test(strat, 10000);

        auto const test_res = test
            .run(ts.vals_for(dataset_name))
            // .run(price_hist)
            .results();

        std::cout << std::endl
                  << "TOTAL RET: " << test_res.growth() << std::endl;

        // std::cout << std::endl
        //           << "HIST: " << st::str_rep(price_hist) << std::endl;
        // std::cout << std::endl
        //           << "PV: " << st::str_rep(test_res.pv()) << std::endl;
        // std::cout << std::endl
        //           << "W: " << st::str_rep(test_res.wt()) << std::endl;
    }

    // SECTION("Buy and hold NDX") {
    //     st::const_alloc buy_and_hold(0.99);
    //     st::single_asset test(buy_and_hold, 10000);

    //     auto const test_res = test
    //         .run(ts.vals_for(dataset_name))
    //         .results();

    //     std::cout << std::endl
    //               << "TOTAL RET: " << test_res.growth() << std::endl;
    // }
}

/*
TEST_CASE("Some new feature", "[usage]") {
    st::price_t price_hist {
        116.5, 13.1, 9.2, 520.4, 12.2, 88.5, 100.0, 800.1, 3.2};

    SECTION("A lookback period of M length") {
        size_t const m_lb = 3;

        auto const test_rule = [](st::price_t pt) {
            auto const pt_sum =
                std::accumulate(pt.begin(), pt.end(),
                                decltype(pt)::value_type(0));
            return pt_sum / 10000;
        };
        auto alloc_rule = new st::lambda_alloc(test_rule);
        auto strat = st::strategy(alloc_rule).look_back(m_lb);

        st::price_t const hist_slice(price_hist.begin(),
                                     price_hist.begin() + 2);

        auto const w = strat.on_hist(hist_slice);

        // std::cout << "   W = " << w << std::endl;
    }
} */
