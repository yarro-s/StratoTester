/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <iostream>
#include <cmath>

#include <allocators/weight_alloc.hpp>
#include <utils.hpp>

#include <catch2/catch.hpp>

SCENARIO("Specified weight allocation",
         "[weight_alloc][asset_alloc]") {
    GIVEN("Price history with corresponding weights") {
        bt::price_ts const price_hist {
            100.0, 200.5,  10.5, 300.1};
        bt::weight_ts const weights {
              0.2,   0.8,  -0.5,   0.7};

        bt::weight_alloc w_alloc(weights);

        WHEN("first price fetched") {
            auto const &p0 = bt::price_ts(
                price_hist.begin(), price_hist.begin()+1);

            REQUIRE(p0.back() == price_hist.front());

            bt::weight const w0 = w_alloc.on_hist(p0);

            THEN("first weight returned") {
                REQUIRE(w0 == weights.front());
            }
        }

        WHEN("third price fetched") {
            auto const &p02 = bt::price_ts(
                price_hist.begin(), price_hist.begin() + 3);

            REQUIRE(p02.back() == price_hist[2]);

            bt::weight const w1 = w_alloc.on_hist(p02);

            THEN("third weight returned") {
                REQUIRE(w1 == weights[2]);
            }
        }

        WHEN("last price fetched") {
            auto const &p0N = bt::price_ts(
                price_hist.begin(), price_hist.end());

            REQUIRE(p0N.back() == price_hist.back());

            bt::weight const wN = w_alloc.on_hist(p0N);

            THEN("last weight returned") {
                REQUIRE(wN == weights.back());
            }
        }
    }
}

SCENARIO("Weights with some incorrect values are given",
        "[weight_alloc][asset_alloc]") {
    GIVEN("Price history and incorrect weights") {
        bt::price_ts const price_hist {
            500.0, 10.5, 1000.1, 850.5,  10};
        bt::weight_ts const weights {
              0.2,  1.4,    150,  -8.5, 0.8};

        bt::weight_alloc w_alloc(weights);

        WHEN("price for an incorrect positive weight fetched") {
            auto const &p01 = bt::price_ts(
                price_hist.begin(), price_hist.begin() + 2);

            REQUIRE(p01.back() == price_hist[1]);

            bt::weight const w0 = w_alloc.on_hist(p01);

            THEN("the incorrect weight clipped to 1") {
                REQUIRE(w0 == 1.0);
            }
        }

        WHEN("price for an incorrect negative weight fetched") {
            auto const &p03 = bt::price_ts(
                price_hist.begin(), price_hist.begin() + 4);

            REQUIRE(p03.back() == price_hist[3]);

            bt::weight const w0 = w_alloc.on_hist(p03);

            THEN("the incorrect weight clipped to -1") {
                REQUIRE(w0 == -1.0);
            }
        }
    }
}

SCENARIO("Weight allocation on randomized weights",
        "[weight_alloc][asset_alloc]") {
    GIVEN("Big history with various weights") {
        bt::price_ts const price_hist {
            100.0, 200.5,  10.5, 300.1, 32.5, 785.3, 43.5, 40.6,
              70.5, 143.4, 345.6, 42.5, 106.5};
        bt::weight_ts const weights {
              0.2,   0.8,  -0.5,   0.7,  2.5,   1.0,  0.7, -1.0,
              -2.5,  -0.8,   0.9,  6.4,  0.5};

        bt::weight_alloc w_alloc(weights);

        WHEN("i-th price fetched") {
            for (auto pT = price_hist.begin() + 1;
                 pT != price_hist.end(); ++pT) {
                auto const &p0T = bt::price_ts(price_hist.begin(), pT);

                bt::weight const wT = w_alloc.on_hist(p0T);

                const auto idxw =
                    std::distance(price_hist.begin(), pT - 1);
                auto w_expected = weights[idxw];

                if (abs(w_expected) > 1.0) {   // clip to ±1.0
                    w_expected = w_expected > 0 ? 1.0 : -1.0;
                }

                THEN("clipped i-th weight returned") {
                    REQUIRE(wT == w_expected);
                }
            }
        }
    }
}
