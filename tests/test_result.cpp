// Copyright 2020 Yarro S

#include <cmath>

#include <iostream>  // debugging
#include <utils.hpp>

#include <balance_book.hpp>
#include <result.hpp>

#include <catch2/catch.hpp>

SCENARIO("Results of transactions", "[result]") {
    GIVEN("Some money") {
        bt::price const init_deposit = 20000;
        bt::balance_book book(init_deposit);
        bt::result res;

        bt::price px = 0;
        size_t amount = 0;

        WHEN("Some asset is bought") {
            px = 105;
            amount = 5;

            book.mkt_price(px).buy(amount);
            res.save(book);

            const auto cash_left = book.cash();

            THEN("the resulting PV is saved") {
                REQUIRE(res.pv().back() == init_deposit);
            }

            THEN("the resulting weight is saved") {
                bt::price const val = px * amount;
                REQUIRE(res.wt().back() == val / init_deposit);
            }

            WHEN("Market price goes up") {
                px = 205;
                auto const new_val = amount * px;

                res.save(book.mkt_price(px));

                THEN("the stored PV goes up") {
                    REQUIRE(res.pv().back() > res.pv()[0]);
                    REQUIRE(res.pv().back() == new_val + cash_left);
                }

                THEN("the stored weight increases") {
                    REQUIRE(res.wt().back() > res.wt()[0]);
                    REQUIRE(res.wt().back() ==
                            new_val / (new_val + cash_left));
                }
            }

            WHEN("Market price goes down") {
                px = 55;
                auto const new_val = amount * px;

                res.save(book.mkt_price(px));

                THEN("the stored PV goes down") {
                    REQUIRE(res.pv().back() < res.pv()[0]);
                    REQUIRE(res.pv().back() == new_val + cash_left);
                }

                THEN("the stored weight decreases") {
                    REQUIRE(res.wt().back() < res.wt()[0]);
                    REQUIRE(res.wt().back() ==
                            new_val / (new_val + cash_left));
                }
            }
        }
    }
}

