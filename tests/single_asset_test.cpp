#include <string>
#include <cmath>
#include <stdlib.h>
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
            10,  12,  14,  19,  16,  14, 120, 125};
        auto const &wT = bt::weight_t{
                0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0};

        auto strat = bt::weight_alloc(wT);
        auto back_test = bt::single_asset(strat, 1000.0);
        auto const &res = back_test.run(pT).results();

        auto const n_0 = trunc(1000.0 / 19.0);
        auto const cash_0 = 1000.0 - n_0 * 19.0;
        auto const val_0 = n_0 * 19.0 + cash_0;
        auto const val_1 = n_0 * 120.0 + cash_0;
        auto const growth_expected = val_1 / val_0;

        // std::cout << std::endl << "Arbitrary weight 1: \n    PV: "
        //     << bt::ts_to_str(res.pv()) << std::endl
        //     << " G = " << growth_expected
        //     << " vs G (actual) = " << res.growth()
        //     << std::endl;

        REQUIRE(Approx(growth_expected) == res.growth());
        
    }

    SECTION( "Arbitrary weight 2" )
    {
        auto const &pT = bt::price_t{
            20.5, 10.5, 12.3, 9.0, 10.4, 10.5, 14.5, 14.5, 46.5};
        auto const &wT = bt::weight_t{
                   0.7,  0.5, 0.5,  0.9,  0.9,  0.3,  0.0,  0.0, 0.0};

        auto strat = bt::weight_alloc(wT);
        auto back_test = bt::single_asset(strat, 1000000);
        auto const &res = back_test.run(pT).results();

        // see single asset model in tests/data/
        auto const growth_expected = 1.4156742;
        REQUIRE(Approx(growth_expected) == res.growth());
        
        // presuming annual weighting
        auto const &timed_res =
            back_test.results(bt::tf::year);

        // time-invariant
        REQUIRE(timed_res.growth() ==   
                Approx(growth_expected));

        auto const cagr_expected = 0.0393784352;
        REQUIRE(Approx(cagr_expected) == timed_res.cagr());
    }

    SECTION( "Constant price and weight" )
    {
        auto const &pT = bt::price_t{14.5, 14.5, 14.5};
        auto const &wT = bt::weight_t{0.3,  0.3,  0.0};

        auto strat = bt::weight_alloc(wT);
        auto back_test = bt::single_asset(strat, 1000000);
        auto const &res = back_test.run(pT).results();

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
        auto const &pT = bt::price_t{546.5, 16.3, 18.0, 18.0, 300.0};
        auto const &wT = bt::weight_t{       0.3,  0.3,  0.0,   0.0};

        auto strat = bt::weight_alloc(wT);
        auto back_test = bt::single_asset(strat, 1000000);
        auto const &res = back_test.run(pT).results();

        // see single asset model in tests/data/
        auto const growth_expected = 1.0312868;
        REQUIRE(res.growth() == Approx(growth_expected));
        
        // presuming annual weighting
        auto const &timed_res =
            back_test.results(bt::tf::year);

        // time-invariant
        REQUIRE(timed_res.growth() ==   
                Approx(growth_expected));

        auto const cagr_expected = 0.0061804895;
        REQUIRE(timed_res.cagr() == Approx(cagr_expected));
    }

    SECTION( "Price down, weight constant" )
    {
        auto const &pT = bt::price_t{235.5, 15.2, 14.6, 14.6, 25.5};
        auto const &wT = bt::weight_t{       0.3,  0.3,  0.0,  0.0};

        auto strat = bt::weight_alloc(wT);
        auto back_test = bt::single_asset(strat, 1000000);
        auto const &res = back_test.run(pT).results();

        // see single asset model in tests/data/
        auto const growth_expected = 0.9881584;
        REQUIRE(res.growth() == Approx(growth_expected));
        
        // presuming annual weighting
        auto const &timed_res =
            back_test.results(bt::tf::year);

        // time-invariant
        REQUIRE(Approx(growth_expected) == timed_res.growth());

        auto const cagr_expected = -0.0023796182;
        REQUIRE(Approx(cagr_expected) == timed_res.cagr());
    }

    SECTION( "Price constant, weight up" )
    {
        auto const &pT = bt::price_t{353.5, 14.6, 14.6, 14.6, 25.6};
        auto const &wT = bt::weight_t{       0.7,  0.8,  0.0,  0.0};

        auto strat = bt::weight_alloc(wT);
        auto back_test = bt::single_asset(strat, 1000000);
        auto const &res = back_test.run(pT).results();

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
        auto const &pT = bt::price_t{53.0, 14.6, 14.6, 14.6, 245.6};
        auto const &wT = bt::weight_t{      0.6,  0.2,  0.0,   0.0};

        auto strat = bt::weight_alloc(wT);
        auto back_test = bt::single_asset(strat, 1000000);
        auto const &res = back_test.run(pT).results();

        // see single asset model in tests/data/
        auto const growth_expected = 1.0;
        REQUIRE(Approx(growth_expected) == res.growth());
        
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
