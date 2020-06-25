#include <string>
#include <math.h>
#include <limits>
#include <iostream>
#include <catch2/catch.hpp>
#include <single_asset.hpp>
#include <strategies/weight_alloc.hpp>
#include <utils.hpp>

TEST_CASE( "4: Backtest result test", "[multi-file:4]" )
{
    auto const &wT = bt::weight_t{
                1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0};

    SECTION( "One peak and small trough" )
    {
        auto const &pT = bt::price_t{
            535.0, 90, 150, 130, 250, 120, 180, 180};

        auto strat = bt::weight_alloc(wT);
        auto back_test = bt::single_asset(strat);
        auto const &res = back_test.run(pT).results();

        auto const max_dd_expected = -0.52;

        REQUIRE(Approx(max_dd_expected) == res.max_drawdown());
    }

    SECTION( "One peak" )
    {
        auto const &pT = bt::price_t{
            2426, 100, 150, 130, 250, 80, 180, 180};

        auto strat = bt::weight_alloc(wT);
        auto back_test = bt::single_asset(strat, 100.0);
        auto const &res = back_test.run(pT).results();

        auto const max_dd_expected = -0.68;

        REQUIRE(Approx(max_dd_expected) == res.max_drawdown());
    }

    SECTION( "Constant price" )
    {
        auto const &pT = bt::price_t{
            10.0, 10.0, 10.0, 10.0, 10.0, 10.0};

        auto strat = bt::weight_alloc(wT);
        auto back_test = bt::single_asset(strat, 100.0);
        auto const &res = back_test.run(pT).results();

        auto const max_dd_expected = 0.0;

        REQUIRE(Approx(max_dd_expected) == res.max_drawdown());
    }

    SECTION( "Inverse course" )
    {
        auto const &pT = bt::price_t{
            150, 220, 250, 130, 145, 170};

        auto strat = bt::weight_alloc(wT);
        auto back_test = bt::single_asset(strat, 1000.0);
        auto const &res = back_test.run(pT).results();

        auto const max_dd_expected = -0.45;

        REQUIRE(Approx(max_dd_expected) == res.max_drawdown());
    }

    SECTION( "Normal course" )
    {
        auto const &pT = bt::price_t{
            10.0, 20.0, 12.0, 15.0, 28.0, 23.0};

        auto strat = bt::weight_alloc(wT);
        auto back_test = bt::single_asset(strat, 100.0);
        auto const &res = back_test.run(pT).results();

        auto const max_dd_expected = -0.4;

        REQUIRE(Approx(max_dd_expected) == res.max_drawdown());
    }

}

