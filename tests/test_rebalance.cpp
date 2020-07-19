// Copyright 2020 Yarro S

#include <iostream>
#include <numeric>

#include <rebalance.hpp>
#include <strategies/lambda_alloc.hpp>
#include <single_asset.hpp>
#include <utils.hpp>

#include <catch2/catch.hpp>

SCENARIO("Rebalancing", "[rebalace]") {
    bt::price_t price_hist {
        16.5, 203.1, 9.2, 400.4, 12.5, 995.0, 3.2, 54.0, 750.4};

    auto const test_rule = [](bt::price_t) {
        return 1.0;
    };

    GIVEN("A rebalancing period of some length") {
        size_t const n_rb = 3;

        auto alloc_rule = new bt::lambda_alloc(test_rule);
        auto strat = bt::rebalance(alloc_rule, n_rb);

        WHEN("history is less then the rebalancing period") {
            bt::price_t const hist_slice(price_hist.begin(),
                                     price_hist.begin() + n_rb - 1);

            THEN("no rebalancing triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(0));
            }
        }

        WHEN("history is equal to the rebalancing period") {
            bt::price_t const hist_slice(price_hist.begin(),
                                     price_hist.begin() + n_rb);

            THEN("rebalancing is triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(1));
            }
        }

        WHEN("history is not a multiple of the rebalancing period") {
            bt::price_t const hist_slice(price_hist.begin(),
                                     price_hist.begin() + n_rb + 1);

            THEN("rebalancing is not triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(0));
            }
        }

        WHEN("history is a multiple of the rebalancing period") {
            bt::price_t const hist_slice(price_hist.begin(),
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
        auto strat = bt::rebalance(alloc_rule, n_rb);

        WHEN("history is equal to the rebalancing period") {
            bt::price_t const hist_slice(price_hist.begin(),
                                     price_hist.begin() + n_rb);

            THEN("rebalancing triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(1));
            }
        }

        WHEN("history is longer then the rebalancing period") {
            bt::price_t const hist_slice(price_hist.begin(),
                                     price_hist.begin() + n_rb + 1);

            THEN("rebalancing triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(1));
            }
        }

        WHEN("history is much longer then the rebalancing period") {
            bt::price_t const hist_slice(price_hist.begin(),
                                     price_hist.begin() + 2*n_rb + 1);

            THEN("rebalancing triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(1));
            }
        }
    }
}
