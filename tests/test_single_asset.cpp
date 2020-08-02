/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <cmath>

#include <iostream>  // debugging
#include <utils.hpp>

#include <single_asset.hpp>
#include <allocators/weight_alloc.hpp>
#include <allocators/const_alloc.hpp>

#include <catch2/catch.hpp>

TEST_CASE("Changing portfolio", "[single_asset][result]") {
    bt::price const init_deposit = 10000;

    SECTION("Arbitrary weights") {
        bt::price const init_cash = 10000;
        bt::prices pT{ 75.2, 125.3, 36.8,  305,  145,  8.5, 45.3};
        bt::weights wT{0.24,  0.26, 0.52, 0.73, 0.61, 0.44, 0.79};

        bt::weight_alloc strat(wT);
        bt::single_asset back_test(strat, init_cash);

        auto res = back_test.run(pT).results();

        auto const pv_expected =
            "[10000, 10000, 8318.5, 23874.1, 17474.1, 5598.6, 20355.4]";

        REQUIRE(pv_expected == bt::str_rep(res.pv()));
    }

    SECTION("Changing weight and asset price") {
        bt::prices pT{ 150, 180, 250};
        bt::weights wT{0.7, 0.8, 0.5};

        bt::weight_alloc strat(wT);
        bt::single_asset back_test(strat, init_deposit);

        auto res = back_test.run(pT).results();

        auto n_asset_expected = trunc(init_deposit * wT[0] / pT[1]);
        auto cash_expected = init_deposit - n_asset_expected * pT[1];

        auto const end_val_expected =
            n_asset_expected * pT.back() + cash_expected;

        REQUIRE(Approx(end_val_expected / init_deposit)
                == res.growth());
    }
}

TEST_CASE("Buy and hold portfolio", "[single_asset][result]") {
    bt::price const init_deposit = 10000;

    SECTION("Constant weight, growing price") {
        bt::prices pT{  150, 180, 250, 300, 410};
        bt::weight w = 0.7;

        bt::const_alloc strat(w);
        bt::single_asset back_test(strat, init_deposit);

        auto res = back_test.run(pT).results();

        auto n_asset_expected = trunc(init_deposit * w / pT[0]);
        auto cash_expected = init_deposit - n_asset_expected * pT[0];

        auto const end_val_expected =
            n_asset_expected * pT.back() + cash_expected;

        CHECK(Approx(end_val_expected / init_deposit)
                == res.growth());
    }

    SECTION("Constant price, constant weight") {
        bt::prices pT{  150, 150, 150};
        bt::weights wT {0.7, 0.7, 0.7};

        bt::weight_alloc strat(wT);
        bt::single_asset back_test(strat, init_deposit);

        auto res = back_test.run(pT).results();

        REQUIRE(1.0 == res.growth());
    }
}
