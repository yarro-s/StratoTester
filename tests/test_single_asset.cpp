// Copyright 2020 Yarro S

#include <cmath>

#include <iostream>  // debugging
#include <utils.hpp>

#include <single_asset.hpp>
#include <strategies/weight_alloc.hpp>

#include <catch2/catch.hpp>

SCENARIO("Weighting the portfolio", "[single_asset]") {
    bt::weight_t wT { 0.0, 0.0, 0.0 };
    bt::weight_alloc dummy_alloc(wT);

    GIVEN("Constant asset weight") {
        bt::price const init_deposit = 10000;
        bt::single_asset model(dummy_alloc, init_deposit);

        bt::weight const wx = 0.7;
        bt::price const px = 750;

        auto const n_actual_asset =
            [init_deposit](bt::price px, bt::weight wx) {
                return trunc((init_deposit * wx) / px);
            };

        int const n_asset = n_actual_asset(px, wx);
        bt::price const init_cash = init_deposit - n_asset * px;

        WHEN("Asset is just allocated") {
            model.update(px, wx);

            THEN("market value stays the same") {
                REQUIRE(model.results().pv().back() == init_deposit);
            }

            THEN("actual weight is corrected for the market") {
                auto const actual_weight =
                    n_asset * px / init_deposit;

                REQUIRE(model.results().wt().back() == actual_weight);
            }

            WHEN("Price goes up") {
                auto const new_px = 1.2 * px;
                model.update(new_px, wx);

                auto const new_val = n_asset * new_px;
                auto const new_pv = init_cash + new_val;

                THEN("market value goes up") {
                    REQUIRE(model.results().pv().back() == new_pv);
                }

                THEN("weight increases") {
                    auto const new_w =
                        new_val / (init_cash + new_val);

                    // REQUIRE(model.results().wt().back() == new_w);
                }
            }

            WHEN("Price goes down") {
                auto const new_px = 0.7 * px;
                model.update(new_px, wx);

                auto const new_val = n_asset * new_px;
                auto const new_pv = init_cash + new_val;

                THEN("market value goes down") {
                    REQUIRE(model.results().pv().back() == new_pv);
                }

                THEN("weight decreases") {
                    auto const new_w =
                        new_val / (init_cash + new_val);

                    // REQUIRE(model.results().wt().back() == new_w);
                }
            }
        }
    }
}
