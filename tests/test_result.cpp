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
            1.0, 1.0, 1.0, 1.0, 1.0, 1.0};

    SECTION( "One peak" )
    {
        auto const &pT = bt::price_t{
            100, 150, 130, 250, 80, 180};

        auto back_test = bt::single_asset(pT, 100.0);
        auto strat = bt::weight_alloc(wT);
        auto const &res = back_test.run(strat).results();

        auto const max_dd_expected = -0.68;

        // std::cout << ":" << bt::ts_to_str(res.pvT) << std::endl;
        REQUIRE(res.max_drawdown() ==
              Approx(max_dd_expected));
    }

    SECTION( "Constant price" )
    {
        auto const &pT = bt::price_t{
            10.0, 10.0, 10.0, 10.0, 10.0, 10.0};

        auto back_test = bt::single_asset(pT, 100.0);
        auto strat = bt::weight_alloc(wT);
        auto const &res = back_test.run(strat).results();

        auto const max_dd_expected = 0.0;

        REQUIRE(res.max_drawdown() ==
              Approx(max_dd_expected));
    }

    SECTION( "Inverse course" )
    {
        auto const &pT = bt::price_t{
            150, 220, 250, 130, 145, 170};

        auto back_test = bt::single_asset(pT, 1000.0);
        auto strat = bt::weight_alloc(wT);
        auto const &res = back_test.run(strat).results();

        auto const max_dd_expected = -0.45;

        REQUIRE(res.max_drawdown() ==
              Approx(max_dd_expected));
    }

    SECTION( "Normal course" )
    {
        auto const &pT = bt::price_t{
            10.0, 20.0, 12.0, 15.0, 28.0, 23.0};

        auto back_test = bt::single_asset(pT, 100.0);
        auto strat = bt::weight_alloc(wT);
        auto const &res = back_test.run(strat).results();

        auto const max_dd_expected = -0.4;

        REQUIRE(res.max_drawdown() ==
              Approx(max_dd_expected));
    }

}

