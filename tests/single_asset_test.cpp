#include <string>
#include <math.h>
#include <limits>
#include <iostream>
#include <catch2/catch.hpp>
#include <single_asset.hpp>
#include <strategies/weight_alloc.hpp>
#include <utils.hpp>

TEST_CASE( "3: Single asset backtest with variable weights", "[multi-file:3]" )
{
    SECTION( "Arbitrary weight 1" )
    {
        auto const &pT = bt::price_t{
            10,   12,   9,  15,  14,  19,  16, 140};
        auto const &wT = bt::weight_t{
            0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0};

        auto back_test = bt::single_asset(pT, 1000.0);
        auto strat = bt::weight_alloc(wT);
        auto const &res = back_test.run(strat).results();

        // see single asset model in tests/data/
        auto const growth_expected = 9.25;
        REQUIRE(res.growth() == Approx(growth_expected));
    }

    SECTION( "Arbitrary weight 2" )
    {
        auto const &pT = bt::price_t{
            10.5, 12.3, 9.0, 10.4, 10.5, 14.5};
        auto const &wT = bt::weight_t{
            0.7, 0.5, 0.5, 0.9, 0.9, 0.3};

        auto back_test = bt::single_asset(pT, 1000000);
        auto strat = bt::weight_alloc(wT);
        auto const &res = back_test.run(strat).results();

        // see single asset model in tests/data/
        auto const growth_expected = 1.3126988;
        REQUIRE(res.growth() == Approx(growth_expected));
        
        // presuming annual weighting
        auto const &timed_res =
            back_test.results(bt::tf::year);

        // time-invariant
        REQUIRE(timed_res.growth() ==   
                Approx(growth_expected));

        auto const cagr_expected = 0.04639174;
        REQUIRE(timed_res.cagr() == Approx(cagr_expected));
    }

    SECTION( "Constant price and weight" )
    {
        auto const &pT = bt::price_t{14.5, 14.5};
        auto const &wT = bt::weight_t{0.3, 0.3};

        auto back_test = bt::single_asset(pT, 1000000);
        auto strat = bt::weight_alloc(wT);
        auto const &res = back_test.run(strat).results();

        // see single asset model in tests/data/
        auto const growth_expected = 1.0;
        REQUIRE(res.growth() == Approx(growth_expected));
        
        // presuming annual weighting
        auto const &timed_res =
            back_test.results(bt::tf::year);

        // time-invariant
        REQUIRE(timed_res.growth() ==   
                Approx(growth_expected));

        auto const cagr_expected = 0.0;
        REQUIRE(timed_res.cagr() == Approx(cagr_expected));
    }

    SECTION( "Price up, weight constant" )
    {
        auto const &pT = bt::price_t{16.3, 18.0};
        auto const &wT = bt::weight_t{0.3, 0.3};

        auto back_test = bt::single_asset(pT, 1000000);
        auto strat = bt::weight_alloc(wT);
        auto const &res = back_test.run(strat).results();

        // see single asset model in tests/data/
        auto const growth_expected = 1.0312868;
        REQUIRE(res.growth() == Approx(growth_expected));
        
        // presuming annual weighting
        auto const &timed_res =
            back_test.results(bt::tf::year);

        // time-invariant
        REQUIRE(timed_res.growth() ==   
                Approx(growth_expected));

        auto const cagr_expected = 0.015522919;
        REQUIRE(timed_res.cagr() == Approx(cagr_expected));
    }

    SECTION( "Price down, weight constant" )
    {
        auto const &pT = bt::price_t{15.2, 14.6};
        auto const &wT = bt::weight_t{0.3, 0.3};

        auto back_test = bt::single_asset(pT, 1000000);
        auto strat = bt::weight_alloc(wT);
        auto const &res = back_test.run(strat).results();

        // see single asset model in tests/data/
        auto const growth_expected = 0.9881584;
        REQUIRE(res.growth() == Approx(growth_expected));
        
        // presuming annual weighting
        auto const &timed_res =
            back_test.results(bt::tf::year);

        // time-invariant
        REQUIRE(timed_res.growth() ==   
                Approx(growth_expected));

        auto const cagr_expected = -0.005938432;
        REQUIRE(timed_res.cagr() == Approx(cagr_expected));
    }

    SECTION( "Price constant, weight up" )
    {
        auto const &pT = bt::price_t{14.6, 14.6};
        auto const &wT = bt::weight_t{0.7, 0.8};

        auto back_test = bt::single_asset(pT, 1000000);
        auto strat = bt::weight_alloc(wT);
        auto const &res = back_test.run(strat).results();

        // see single asset model in tests/data/
        auto const growth_expected = 1.0;
        REQUIRE(res.growth() == Approx(growth_expected));
        
        // presuming annual weighting
        auto const &timed_res =
            back_test.results(bt::tf::year);

        // time-invariant
        REQUIRE(timed_res.growth() ==   
                Approx(growth_expected));

        auto const cagr_expected = 0.0;
        REQUIRE(timed_res.cagr() == Approx(cagr_expected));
    }

    SECTION( "Price constant, weight down" )
    {
        auto const &pT = bt::price_t{14.6, 14.6};
        auto const &wT = bt::weight_t{0.6, 0.2};

        auto back_test = bt::single_asset(pT, 1000000);
        auto strat = bt::weight_alloc(wT);
        auto const &res = back_test.run(strat).results();

        // see single asset model in tests/data/
        auto const growth_expected = 1.0;
        REQUIRE(res.growth() == Approx(growth_expected));
        
        // presuming annual weighting
        auto const &timed_res =
            back_test.results(bt::tf::year);

        // time-invariant
        REQUIRE(timed_res.growth() ==   
                Approx(growth_expected));

        auto const cagr_expected = 0.0;
        REQUIRE(timed_res.cagr() == Approx(cagr_expected));
    }
}
