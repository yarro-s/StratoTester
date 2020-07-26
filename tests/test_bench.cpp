/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <iostream>
#include <numeric>
#include <catch2/catch.hpp>

#include <strategy.hpp>
#include <single_asset.hpp>
#include <allocators/lambda_alloc.hpp>
#include <utils.hpp>


TEST_CASE("New feature", "[usage]") {
    bt::price_t price_hist {
        116.5, 13.1, 9.2, 520.4, 12.2, 88.5, 100.0, 800.1, 3.2};

    SECTION("A lookback period of M length") {
        size_t const m_lb = 3;

        auto const test_rule = [](bt::price_t pt) {
            auto const pt_sum =
                std::accumulate(pt.begin(), pt.end(),
                                decltype(pt)::value_type(0));
            return pt_sum / 10000;
        };
        auto alloc_rule = new bt::lambda_alloc(test_rule);
        auto strat = bt::strategy(alloc_rule).look_back(m_lb);

        bt::price_t const hist_slice(price_hist.begin(),
                                     price_hist.begin() + 2);

        auto const w = strat.on_hist(hist_slice);

        std::cout << "   W = " << w << std::endl;
    }
}
