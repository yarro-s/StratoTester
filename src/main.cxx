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

int main()
{
    bt::price_t pT {
        20.5, 10.5, 12.3, 9.0, 10.4, 10.5, 14.5, 14.5, 46.5};
    bt::weight_t wT {
        0.7,  0.5, 0.5,  0.9,  0.9,  0.3,  0.0,  0.0, 0.0};

    // auto base_strat = bt::weight_alloc(wT);
    auto strat = bt::strategy(new bt::weight_alloc(wT)).lookback(3)
        .rebalance_every(4);

    // bt::weight_alloc strat0(wT);
    // bt::asset_alloc_lb strat1(strat0, 3);
    // bt::asset_alloc_rb strat(strat1, 4);

    // bt::strategy strat0 = bt::strategy(bt::weight_alloc(wT));
    // bt::strategy strat = bt::strategy(bt::asset_alloc_lb(strat0, 3));

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