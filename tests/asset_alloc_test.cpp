#include <string>
#include <math.h>
#include <numeric>
#include <catch2/catch.hpp>
#include <single_asset.hpp>
#include <strategies/ma_cross.hpp>

TEST_CASE("5: Asset allocation with lookback",
          "[multi-file:5]")
{
    auto const &pT = bt::price_t{
            10, 12, 9, 15, 14, 19, 16, 140};

    SECTION( "Short-long MA cross" )  // TO DO: implement
    {
        size_t m_lb = 3,
           n_lb = 5;

        auto const sma_cross =
            [m_lb, n_lb](bt::price_t const &pT) {
            
                auto const sma = [pT](size_t k_lb) {

                auto const lb_begin = pT.begin() - k_lb;
                    return std::accumulate(lb_begin, 
                        pT.end(), 0) / pT.size();
            };

            return sma(m_lb) > sma(n_lb) ? 1.0 : -1.0;
        };

        auto back_test = bt::single_asset(pT, 1000);
        auto sma = bt::asset_alloc_lb(n_lb, sma_cross);

        auto const &res = back_test.run(sma).results();

        // see MA model in tests/model/
        auto const growth_expected = -7.946;
        REQUIRE(res.growth() == Approx(growth_expected));
    }

    SECTION( "MA cross" )
    {
        size_t n_lookback = 3;

        auto back_test = bt::single_asset(pT, 1000);
        auto sma = bt::ma_cross(n_lookback);
        auto const &res = back_test.run(sma).results();

        // see MA model in tests/model/
        auto const growth_expected = 9.25;
        REQUIRE(res.growth() == Approx(growth_expected));
    }

    SECTION( "MA cross price with lambdas" )
    {
        size_t n_lookback = 3;
        auto const fa = [](bt::price_t const &pT)
        {
            auto const sma = 
                std::accumulate(pT.begin(), pT.end(), 0) 
                    / pT.size();

            auto const p_last = pT.back();
            auto const wT = p_last > sma ? 1.0 : 0.0;
            
            return wT; 
        };

        auto back_test = bt::single_asset(pT, 1000);
        auto sma = bt::asset_alloc_lb(n_lookback, fa);
        auto const &res = back_test.run(sma).results();

        // see MA model in tests/model/
        auto const growth_expected = 9.25;
        REQUIRE(res.growth() == Approx(growth_expected));
    }

}

