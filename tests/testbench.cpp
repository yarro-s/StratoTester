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
#include <utils.hpp>
#include <t_series.hpp>

bt::t_series<bt::price> read_from_csv(
    std::string const &datapath,
    std::string const &dataset_name,
    std::string const &index_name,
    std::string const &field_name,
    std::string const &time_fmt = "%Y-%m-%d") {


    bt::t_series<bt::price> ts;

    io::CSVReader<2, io::trim_chars<>>
        in(datapath + dataset_name + ".csv");

    in.read_header(io::ignore_extra_column,
        index_name, field_name);

    std::string date;
    bt::price px;

    while (in.read_row(date, px)) {
        auto const t_stamp = bt::str_to_time(date, time_fmt);

        ts.append_at(dataset_name, t_stamp, px);
    }

    return ts;
}


TEST_CASE("Real data", "[usage]") {
    std::string const datapath = "../tests/data/";

    bt::t_series<bt::price> ts;

    SECTION("NDX") {
        std::string const dataset_name("^IXIC");

        bt::t_series<bt::price> ts(read_from_csv("../tests/data/",
            dataset_name, "Date", "Close", "%Y-%m-%d"));

        for (auto const &t_stamp : ts.timing()) {
            std::cout << "   " << bt::str_rep(t_stamp);
        }
    }
}


// TEST_CASE("Some new feature", "[usage]") {
//     bt::price_t price_hist {
//         116.5, 13.1, 9.2, 520.4, 12.2, 88.5, 100.0, 800.1, 3.2};

//     SECTION("A lookback period of M length") {
//         size_t const m_lb = 3;

//         auto const test_rule = [](bt::price_t pt) {
//             auto const pt_sum =
//                 std::accumulate(pt.begin(), pt.end(),
//                                 decltype(pt)::value_type(0));
//             return pt_sum / 10000;
//         };
//         auto alloc_rule = new bt::lambda_alloc(test_rule);
//         auto strat = bt::strategy(alloc_rule).look_back(m_lb);

//         bt::price_t const hist_slice(price_hist.begin(),
//                                      price_hist.begin() + 2);

//         auto const w = strat.on_hist(hist_slice);

//         // std::cout << "   W = " << w << std::endl;
//     }
// }
