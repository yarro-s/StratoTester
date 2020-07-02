#include <iostream>
#include <vector>
#include <unistd.h>
#define BACKTESTER_IMPL
// #include <backtester.hpp>
#include <strategies/weight_alloc.hpp>
#include <asset_alloc_lb.hpp>
#include <asset_alloc_rb.hpp>
#include <utils.hpp>
#include <single_asset.hpp>
#include <strategy.hpp>

#include <csv.h>

#define REL_PATH
// #define TEST_REV_REL

int main()
{
    bt::price_t pT {
        110.5, 113.1, 29.0, 220.4, 535.2, 58.5, 100.0, 200.1, 6.2};
    bt::weight_t wT {
          0.2,   0.8,  0.3,   0.4,   0.7,  0.2,   0.0,   0.9, 0.1};

    size_t n = 3, m = 2;
#ifdef REL_PATH
    auto strat = bt::strategy<bt::weight_alloc>(wT)
                     .lookback(m)
                     .rebalance_every(n);
#endif
#ifdef TEST_REV_REL
    auto strat = bt::strategy<bt::weight_alloc>(wT)
                     .rebalance_every(n)
                     .lookback(m);
#endif

    bt::single_asset back_test(strat);
    auto const &res = back_test.run(pT);
}

/*
int main0()
{
    bt::price_t pT {
        20.5, 10.5, 12.3, 9.0, 10.4, 10.5, 14.5, 14.5, 46.5};
    bt::weight_t wT {
        0.7,  0.5, 0.5,  0.9,  0.9,  0.3,  0.0,  0.0, 0.0};

    bt::weight_alloc strat(wT);
    bt::single_asset back_test(strat, 1000000);
    auto const &res = back_test.run(pT).results();

    auto const growth_expected = 1.4156742;
    
    auto const &timed_res =
        back_test.results(bt::tf::year);

    std::cout << std::endl
              << "Arbitrary weight 1: \n    PV: "
              << bt::ts_to_str(res.pv()) 
              << " G = " << growth_expected
              << " vs G (actual) = " << res.growth()
              << std::endl;

    return 0;
}*/