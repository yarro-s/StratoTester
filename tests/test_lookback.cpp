// Copyright 2020 Yarro S

#include <iostream>
#include <numeric>

#include <lookback.hpp>
#include <strategies/lambda_alloc.hpp>
#include <single_asset.hpp>
#include <utils.hpp>

#include <catch2/catch.hpp>

SCENARIO("Making decisions on some previous data points",
         "[lookback]") {
    bt::price_t price_hist {
        116.5, 13.1, 9.2, 520.4, 12.2, 88.5, 100.0, 800.1, 3.2};

    GIVEN("A lookback period of M length") {
        size_t const m_lb = 3;

        auto const trading_rule = [](bt::price_t pt) {
            auto const pt_sum =
                std::accumulate(pt.begin(), pt.end(),
                                decltype(pt)::value_type(0));
            return pt_sum / 10000;
        };
        auto alloc_rule = new bt::lambda_alloc(trading_rule);
        auto strat = bt::lookback(alloc_rule, m_lb);

        WHEN("looking back on a history of k points < M lookback") {
            bt::price_t const hist_slice(price_hist.begin(),
                                         price_hist.begin() + 2);

            THEN("the trading rule is not applied") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(0));
            }
        }

        WHEN("looking back on a history of l points = M lookback") {
            bt::price_t const hist_slice(price_hist.begin(),
                                         price_hist.begin() + m_lb);

            THEN("the trading rule is applied to first M samples") {
                auto const w = strat.on_hist(hist_slice);

                auto const w_expected = (price_hist[0] + price_hist[1]
                    + price_hist[m_lb - 1]) / 10000;

                REQUIRE(w == Approx(w_expected));
            }
        }

        WHEN("looking back on a history of n points >> M lookback") {
            auto const n = 2 * m_lb;
            bt::price_t const hist_slice(price_hist.begin(),
                                         price_hist.begin() + n);

            THEN("the trading rule is applied to last M samples") {
                auto const w = strat.on_hist(hist_slice);

                auto const w_expected = (price_hist[n - 3] +
                    price_hist[n - 2] + price_hist[n - 1]) / 10000;

                REQUIRE(w == Approx(w_expected));
            }
        }
    }

    GIVEN("A unit length lookback period") {
        size_t const m_lb = 1;

        auto const trading_rule = [](bt::price_t pt) {
            return pt.front() / 1000;
        };
        auto alloc_rule = new bt::lambda_alloc(trading_rule);
        auto strat = bt::lookback(alloc_rule, m_lb);

        WHEN("looking back on a unit length history") {
            bt::price_t const hist_slice(price_hist.begin(),
                                         price_hist.begin() + 1);

            THEN("only the first data point is taken into account") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(hist_slice.front() / 1000));
            }
        }

        WHEN("looking back on a history > the unit lookback period") {
            bt::price_t const hist_slice(price_hist.begin(),
                                         price_hist.begin() + 2);

            THEN("only the last data point is taken into account") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(hist_slice.back() / 1000));
            }
        }

        WHEN("looking back on a history >> the unit lookback") {
            bt::price_t const hist_slice(price_hist.begin(),
                                         price_hist.begin() + 4);

            THEN("only the last data point is taken into account") {
                auto const w = strat.on_hist(hist_slice);

                REQUIRE(w == Approx(hist_slice.back() / 1000));
            }
        }
    }
}
