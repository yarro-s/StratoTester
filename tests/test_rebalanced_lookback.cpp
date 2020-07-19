// Copyright 2020 Yarro S

#include <iostream>
#include <numeric>

#include <rebalance.hpp>
#include <lookback.hpp>
#include <allocators/lambda_alloc.hpp>
#include <utils.hpp>

#include <catch2/catch.hpp>

SCENARIO("Rebalancing with lookback", "[rebalace][lookback]") {
    bt::price_t price_hist {
        6.5, 253.1, 39.2, 1.4, 555.0, 8.3, 200.0, 9.2, 855.3};

    auto const test_rule = [](bt::price_t pt) {
        auto const pt_sum =
            std::accumulate(pt.begin(), pt.end(),
                            decltype(pt)::value_type(0));
        return pt_sum / 10000;
    };

    GIVEN("Rebalancing with lookback") {
        size_t const n_rb = 3;
        size_t const m_lb = 2;

        auto alloc_rule = new bt::lambda_alloc(test_rule);
        auto strat = bt::rebalance(alloc_rule, n_rb);
        strat.set_lookback(m_lb);

        WHEN("history is less then the rebalancing period") {
            bt::price_t const hist_slice(price_hist.begin(),
                                     price_hist.begin() + n_rb - 1);

            THEN("no rebalancing is triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(0));
            }
        }

        WHEN("history length is equal to the rebalancing period") {
            auto const idxT = n_rb;
            bt::price_t const hist_slice(price_hist.begin(),
                                         price_hist.begin() + idxT);

            THEN("rebalancing is triggered") {
                auto const w = strat.on_hist(hist_slice);
                auto w_expected = (price_hist[idxT - 2] +
                    price_hist[idxT - 1]) / 10000;

                REQUIRE(w == Approx(w_expected));
            }
        }

        WHEN("history is longer then the rebalancing period") {
            bt::price_t const hist_slice(price_hist.begin(),
                price_hist.begin() + n_rb + 1);

            THEN("rebalancing is not triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(0));
            }
        }

        WHEN("history len is a multiple of the rebalancing period") {
            auto const idxT = 2 * n_rb;
            bt::price_t const hist_slice(price_hist.begin(),
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
            bt::price_t const hist_slice(price_hist.begin(),
                price_hist.begin() + idxT);

            THEN("rebalancing is triggered") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(0));
            }
        }
    }
}
