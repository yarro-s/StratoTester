/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <cmath>

#include <iostream>  // debugging
#include <utils.hpp>

#include <balance_book.hpp>
#include <result.hpp>

#include <catch2/catch.hpp>

SCENARIO("Results of transactions", "[result]") {
    GIVEN("Some money") {
        st::price const init_deposit = 20000;
        st::balance_book book(init_deposit);
        st::result res;

        st::price px = 0;
        size_t amount = 0;

        WHEN("Some asset is bought") {
            px = 105;
            amount = 5;

            book.mkt_price(px).buy(amount);
            res.save(book);

            const auto cash_left = book.cash();

            THEN("the resulting PV is saved") {
                REQUIRE(res.value_history().back() == init_deposit);
            }

            THEN("the resulting weight is saved") {
                st::price const val = px * amount;
                REQUIRE(res.asset_weights().back() == val / init_deposit);
            }

            WHEN("Market price goes up") {
                px = 205;
                auto const new_val = amount * px;

                res.save(book.mkt_price(px));

                THEN("the stored PV goes up") {
                    REQUIRE(res.value_history().back() > res.value_history()[0]);
                    REQUIRE(res.value_history().back() == new_val + cash_left);
                }

                THEN("the stored weight increases") {
                    REQUIRE(res.asset_weights().back() > res.asset_weights()[0]);
                    REQUIRE(res.asset_weights().back() ==
                            new_val / (new_val + cash_left));
                }
            }

            WHEN("Market price goes down") {
                px = 55;
                auto const new_val = amount * px;

                res.save(book.mkt_price(px));

                THEN("the stored PV goes down") {
                    REQUIRE(res.value_history().back() < res.value_history()[0]);
                    REQUIRE(res.value_history().back() == new_val + cash_left);
                }

                THEN("the stored weight decreases") {
                    REQUIRE(res.asset_weights().back() < res.asset_weights()[0]);
                    REQUIRE(res.asset_weights().back() ==
                            new_val / (new_val + cash_left));
                }
            }
        }
    }
}

