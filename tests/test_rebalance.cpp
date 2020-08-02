/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <iostream>

#include <strategy.hpp>
#include <allocators/lambda_alloc.hpp>
#include <utils.hpp>

#include <catch2/catch.hpp>

SCENARIO("Rebalancing", "[rebalace]") {
    bt::prices price_hist {
        16.5, 203.1, 9.2, 400.4, 12.5, 995.0, 3.2, 54.0, 750.4};

    auto const test_rule = [](bt::prices) {
        return 1.0;
    };

    GIVEN("A rebalancing period of some length") {
        size_t const n_rb = 3;

        auto alloc_rule = new bt::lambda_alloc(test_rule);
        auto strat = bt::strategy(alloc_rule).rebalance_every(n_rb);

        WHEN("history is less then the rebalancing period") {
            bt::prices const hist_slice(price_hist.begin(),
                                     price_hist.begin() + n_rb - 1);

            THEN("no rebalancing triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(0));
            }
        }

        WHEN("history is equal to the rebalancing period") {
            bt::prices const hist_slice(price_hist.begin(),
                                     price_hist.begin() + n_rb);

            THEN("rebalancing is triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(1));
            }
        }

        WHEN("history is not a multiple of the rebalancing period") {
            bt::prices const hist_slice(price_hist.begin(),
                                     price_hist.begin() + n_rb + 1);

            THEN("rebalancing is not triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(0));
            }
        }

        WHEN("history is a multiple of the rebalancing period") {
            bt::prices const hist_slice(price_hist.begin(),
                                     price_hist.begin() + 2*n_rb);

            THEN("rebalancing is triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(1));
            }
        }
    }

    GIVEN("A unit rebalancing period") {
        size_t const n_rb = 1;

        auto alloc_rule = new bt::lambda_alloc(test_rule);
        auto strat = bt::strategy(alloc_rule).rebalance_every(n_rb);

        WHEN("history is equal to the rebalancing period") {
            bt::prices const hist_slice(price_hist.begin(),
                                     price_hist.begin() + n_rb);

            THEN("rebalancing triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(1));
            }
        }

        WHEN("history is longer then the rebalancing period") {
            bt::prices const hist_slice(price_hist.begin(),
                                     price_hist.begin() + n_rb + 1);

            THEN("rebalancing triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(1));
            }
        }

        WHEN("history is much longer then the rebalancing period") {
            bt::prices const hist_slice(price_hist.begin(),
                                     price_hist.begin() + 2*n_rb + 1);

            THEN("rebalancing triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(1));
            }
        }
    }
}
