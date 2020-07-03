// Copyright 2020 Yaroslav Sh.

#include <iostream>

#include <strategy.hpp>
#include <strategies/weight_alloc.hpp>
#include <single_asset.hpp>
#include <utils.hpp>

#include <catch2/catch.hpp>


TEST_CASE("Basic usage showcase", "[usage]") {
    bt::price_t price_hist {
        110.5, 113.1, 29.0, 220.4, 535.2, 58.5, 100.0, 200.1, 6.2};
    bt::weight_t weights {
          0.2,   0.8,  0.3,   0.4,   0.7,  0.2,   0.0,   0.9, 0.1};

    SECTION("backtest with lookback and rebalancing is run") {
        size_t n_rebalance = 3, m_lookback = 2;

        auto strat = bt::strategy<bt::weight_alloc>(weights)
                         .lookback(m_lookback)
                         .rebalance_every(n_rebalance);

        bt::single_asset back_test(strat, 10000);
        auto const &res = back_test.run(price_hist).results();

        // Checks
        CHECK(price_hist.size() == res.pv().size());

        auto const pv0_expected = 10000.0,
                   pvT_expected = 38349.8;

        CHECK(Approx(pv0_expected) == res.pv().front());
        CHECK(Approx(pvT_expected) == res.pv().back());
    }
}
