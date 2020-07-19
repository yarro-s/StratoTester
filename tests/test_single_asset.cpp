// Copyright 2020 Yarro S

#include <cmath>

#include <iostream>  // debugging
#include <utils.hpp>

#include <single_asset.hpp>
#include <allocators/weight_alloc.hpp>
#include <buy_and_hold.hpp>

#include <catch2/catch.hpp>

TEST_CASE("Changing portfolio", "[single_asset][result]") {
    bt::price const init_deposit = 10000;

    SECTION("Changing weight and asset price") {
        bt::price_t pT{ 150, 180, 250};
        bt::weight_t wT{0.7, 0.8, 0.5};

        bt::weight_alloc strat(wT);
        bt::single_asset back_test(strat, init_deposit);

        auto res = back_test.run(pT).results();

        auto n_asset_expected = trunc(init_deposit * wT[0] / pT[1]);
        auto cash_expected = init_deposit - n_asset_expected * pT[1];

        auto const end_val_expected =
            n_asset_expected * pT.back() + cash_expected;

        CHECK(Approx(end_val_expected / init_deposit)
                == res.growth());
    }
}

TEST_CASE("Buy and hold portfolio", "[single_asset][result]") {
    bt::price const init_deposit = 10000;

    SECTION("Constant weight, growing price") {
        bt::price_t pT{  150, 180, 250, 300, 410};
        bt::weight w = 0.7;

        bt::buy_and_hold strat(w);
        bt::single_asset back_test(strat, init_deposit);

        auto res = back_test.run(pT).results();

        auto n_asset_expected = trunc(init_deposit * w / pT[1]);
        auto cash_expected = init_deposit - n_asset_expected * pT[1];

        auto const end_val_expected =
            n_asset_expected * pT.back() + cash_expected;

        CHECK(Approx(end_val_expected / init_deposit)
                == res.growth());
    }

    SECTION("Constant price, constant weight") {
        bt::price_t pT{  150, 150, 150};
        bt::weight_t wT {0.7, 0.7, 0.7};

        bt::weight_alloc strat(wT);
        bt::single_asset back_test(strat, init_deposit);

        auto res = back_test.run(pT).results();

        REQUIRE(1.0 == res.growth());
    }
}
