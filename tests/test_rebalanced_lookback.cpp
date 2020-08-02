/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <iostream>
#include <numeric>

#include <strategy.hpp>
#include <allocators/lambda_alloc.hpp>
#include <utils.hpp>

#include <catch2/catch.hpp>

SCENARIO("Rebalancing with lookback", "[rebalace][lookback]") {
    bt::prices price_hist {
        6.5, 253.1, 39.2, 1.4, 555.0, 8.3, 200.0, 9.2, 855.3};

    auto const test_rule = [](bt::prices pt) {
        auto const pt_sum =
            std::accumulate(pt.begin(), pt.end(),
                            decltype(pt)::value_type(0));
        return pt_sum / 10000;
    };

    GIVEN("Rebalancing with lookback") {
        size_t const n_rb = 3;
        size_t const m_lb = 2;

        auto alloc_rule = new bt::lambda_alloc(test_rule);
        auto strat = bt::strategy(alloc_rule)
            .rebalance_every(n_rb).look_back(m_lb);

        WHEN("history is less then the rebalancing period") {
            bt::prices const hist_slice(price_hist.begin(),
                                     price_hist.begin() + n_rb - 1);

            THEN("no rebalancing is triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(0));
            }
        }

        WHEN("history length is equal to the rebalancing period") {
            auto const idxT = n_rb;
            bt::prices const hist_slice(price_hist.begin(),
                                         price_hist.begin() + idxT);

            THEN("rebalancing is triggered") {
                auto const w = strat.on_hist(hist_slice);
                auto w_expected = (price_hist[idxT - 2] +
                    price_hist[idxT - 1]) / 10000;

                REQUIRE(w == Approx(w_expected));
            }
        }

        WHEN("history is longer then the rebalancing period") {
            bt::prices const hist_slice(price_hist.begin(),
                price_hist.begin() + n_rb + 1);

            THEN("rebalancing is not triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(0));
            }
        }

        WHEN("history len is a multiple of the rebalancing period") {
            auto const idxT = 2 * n_rb;
            bt::prices const hist_slice(price_hist.begin(),
                price_hist.begin() + idxT);

            THEN("rebalancing is triggered") {
                auto const w = strat.on_hist(hist_slice);
                auto w_expected = (price_hist[idxT - 2] +
                    price_hist[idxT - 1]) / 10000;

                REQUIRE(w == Approx(w_expected));
            }
        }

        WHEN("history is not a multiple of the rebalancing period") {
            auto const idxT = 2 * n_rb + 2;
            bt::prices const hist_slice(price_hist.begin(),
                price_hist.begin() + idxT);

            THEN("rebalancing is triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(0));
            }
        }
    }
}
