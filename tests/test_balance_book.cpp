// Copyright 2020 Yarro S

#include <iostream>  // debugging
#include <utils.hpp>

#include <balance_book.hpp>

#include <catch2/catch.hpp>

SCENARIO("Selling and buying in volatility", "[balance_book]") {
    GIVEN("Some money and assets") {
        bt::price const init_deposit = 20000;
        bt::balance_book book(init_deposit);

        bt::price px = 750;
        book.mkt_price(px);

        int const init_assets = 15;
        book.buy(init_assets);

        int n_assets = 0;

        WHEN("Buy after price has gone down") {
            n_assets = 5;
            bt::price new_px = 0.8 * px;

            auto const prev_cash = book.cash();
            auto const prev_assets = book.n_asset();

            book.mkt_price(new_px).buy(n_assets);

            THEN("Assets has gone up") {
                REQUIRE(book.n_asset() == prev_assets + n_assets);
            }

            THEN("Cash has gone down") {
                auto const cash_delta = n_assets * new_px;

                REQUIRE(book.cash() == prev_cash - cash_delta);
            }

            THEN("Market values has gone down") {
                auto const px_delta = new_px - px;
                auto const val_delta = init_assets * px_delta;

                REQUIRE(book.mkt_value() == init_deposit + val_delta);
            }
        }

        WHEN("Sell after price has gone up") {
            n_assets = 5;
            bt::price new_px = 1.2 * px;

            auto const prev_cash = book.cash();
            auto const prev_assets = book.n_asset();

            book.mkt_price(new_px).sell(n_assets);

            THEN("Assets has gone down") {
                REQUIRE(book.n_asset() == prev_assets - n_assets);
            }

            THEN("Cash has gone up") {
                auto const cash_delta = n_assets * new_px;

                REQUIRE(book.cash() == prev_cash + cash_delta);
            }

            THEN("Market values has gone up") {
                auto const px_delta = new_px - px;
                auto const val_delta = init_assets * px_delta;

                REQUIRE(book.mkt_value() == init_deposit + val_delta);
            }
        }
    }
}

SCENARIO("Market price volatility", "[balance_book]") {
    GIVEN("Some money and assets") {
        bt::price const init_deposit = 20000;
        bt::balance_book book(init_deposit);

        bt::price px = 750;
        book.mkt_price(px);

        int const init_assets = 15;
        book.buy(init_assets);

        WHEN("price goes up") {
            auto const cash_before = book.cash();
            auto const n_assets_before = book.n_asset();
            auto const mkt_val_before = book.mkt_value();
            auto const px_before = book.mkt_price();

            px = 1000;
            book.mkt_price(px);

            THEN("the market value goes up") {
                auto const mkt_val_delta =
                    init_assets * (px - px_before);

                REQUIRE(book.mkt_value() ==
                        mkt_val_before + mkt_val_delta);
            }

            THEN("the cash doesn't change") {
                REQUIRE(book.cash() == cash_before);
            }

            THEN("the assets increase by the amount bought") {
                REQUIRE(book.n_asset() == n_assets_before);
            }
        }

        WHEN("price goes down") {
            auto const cash_before = book.cash();
            auto const n_assets_before = book.n_asset();
            auto const mkt_val_before = book.mkt_value();
            auto const px_before = book.mkt_price();

            px = 300;
            book.mkt_price(px);

            THEN("the market value goes up") {
                auto const mkt_val_delta =
                    init_assets * (px - px_before);

                REQUIRE(book.mkt_value() ==
                        mkt_val_before + mkt_val_delta);
            }

            THEN("the cash doesn't change") {
                REQUIRE(book.cash() == cash_before);
            }

            THEN("the assets increase by the amount bought") {
                REQUIRE(book.n_asset() == n_assets_before);
            }
        }
    }
}

SCENARIO("Buying assets", "[balance_book]") {
    GIVEN("No money and no assets") {
        bt::price const init_deposit = 0;
        bt::balance_book book(init_deposit);

        int n_asset = 0;

        WHEN("buying without the market price specified") {
            n_asset = 5;
            book.buy(n_asset);

            THEN("market value of the book hasn't changed") {
                REQUIRE(book.mkt_value() == init_deposit);
            }

            THEN("the amount of cash hasn't changed") {
                REQUIRE(book.cash() == init_deposit);
            }

            THEN("the amount of asset equals the amount bought") {
                REQUIRE(book.n_asset() == n_asset);
            }
        }

        WHEN("buying at the market price") {
            book.mkt_price(43);
            n_asset = 15;
            book.buy(n_asset);

            THEN("market value of the book hasn't changed") {
                REQUIRE(book.mkt_value() == init_deposit);
            }

            THEN("the amount of cash hasn't changed") {
                REQUIRE(book.cash() == init_deposit);
            }

            THEN("the amount of assets is zero") {
                REQUIRE(book.n_asset() == 0);
            }
        }
    }

    GIVEN("Some money and no assets") {
        bt::price const init_deposit = 10000;
        bt::balance_book book(init_deposit);

        int n_asset = 0;

        WHEN("buying without the market price specified") {
            n_asset = 5;
            book.buy(n_asset);

            THEN("market value of the book hasn't changed") {
                REQUIRE(book.mkt_value() == init_deposit);
            }

            THEN("the amount of cash hasn't changed") {
                REQUIRE(book.cash() == init_deposit);
            }

            THEN("the amount of asset equals the amount bought") {
                REQUIRE(book.n_asset() == n_asset);
            }
        }

        WHEN("buying a small amount relative to the deposit") {
            bt::price px = 100;

            book.mkt_price(px);
            n_asset = 3;
            book.buy(n_asset);

            THEN("market value of the book hasn't changed") {
                REQUIRE(book.mkt_value() == init_deposit);
            }

            THEN("the amount of cash has decreased") {
                REQUIRE(book.cash() == init_deposit - n_asset * px);
            }

            THEN("the amount of asset equals the amount bought") {
                REQUIRE(book.n_asset() == n_asset);
            }
        }

        /// TODO: also add boundary checks

        WHEN("buying an amount with value equal to the deposit") {
            bt::price px = 100;

            book.mkt_price(px);
            n_asset = init_deposit / px;
            book.buy(n_asset);

            THEN("market value of the book hasn't changed") {
                REQUIRE(book.mkt_value() == init_deposit);
            }

            THEN("the amount of cash is zero") {
                REQUIRE(book.cash() == Approx(0.0));
            }

            THEN("the amount of asset equals the amount bought") {
                REQUIRE(book.n_asset() == n_asset);
            }
        }

        WHEN("trying to buy more than affordable") {
            bt::price px = 150;

            book.mkt_price(px);
            n_asset = (init_deposit / px) * 3;
            book.buy(n_asset);

            THEN("the amount of cash hasn't changed") {
                REQUIRE(book.cash() == init_deposit);
            }

            THEN("no asset added - transaction rejected") {
                REQUIRE(book.n_asset() == 0);
            }
        }
    }

    GIVEN("Some money and assets") {
        bt::price const init_deposit = 10000;
        bt::balance_book book(init_deposit);

        bt::price px = 150;
        book.mkt_price(px);

        int const init_assets = 20;
        book.buy(init_assets);

        int n_asset = 0;

        WHEN("buying more assets") {
            auto const cash_before = book.cash();
            n_asset = 3;
            book.buy(n_asset);

            THEN("the cash has decreased") {
                REQUIRE(book.cash() == cash_before - n_asset * px);
            }

            THEN("the assets increased by the amount bought") {
                REQUIRE(book.n_asset() == init_assets + n_asset);
            }

            THEN("the market value of the book hasn't changed") {
                REQUIRE(book.mkt_value() == init_deposit);
            }
        }
    }
}

SCENARIO("Selling assets", "[balance_book]") {
    GIVEN("No money and no assets") {
        auto const init_deposit = 0.0;
        bt::balance_book book(init_deposit);

        int n_asset = 0;

        WHEN("selling without the market price specified") {
            n_asset = 4;
            book.sell(n_asset);

            THEN("market value of the book hasn't changed") {
                REQUIRE(book.mkt_value() == init_deposit);
            }

            THEN("the amount of cash hasn't changed") {
                REQUIRE(book.cash() == init_deposit);
            }

            THEN("the amount of asset equals minus the amount sold") {
                REQUIRE(book.n_asset() == -n_asset);
            }
        }
    }

    GIVEN("Some money and no assets") {
        auto const init_deposit = 1000.0;
        bt::balance_book book(init_deposit);

        int n_asset = 0;

        WHEN("selling without the market price specified") {
            n_asset = 4;
            book.sell(n_asset);

            THEN("market value of the book hasn't changed") {
                REQUIRE(book.mkt_value() == init_deposit);
            }

            THEN("the amount of cash hasn't changed") {
                REQUIRE(book.cash() == init_deposit);
            }

            THEN("the amount of asset equals minus the amount sold") {
                REQUIRE(book.n_asset() == -n_asset);
            }
        }

        WHEN("selling at some market price") {
            auto const px = 55;
            book.mkt_price(px);

            n_asset = 10;
            book.sell(n_asset);

            THEN("market value of the book hasn't changed") {
                REQUIRE(book.mkt_value() == init_deposit);
            }

            THEN("the amount of cash increased") {
                REQUIRE(book.cash() == init_deposit + n_asset * px);
            }

            THEN("the amount of asset equals minus the amount sold") {
                REQUIRE(book.n_asset() == -n_asset);
            }
        }

        WHEN("selling more than affordable") {
            auto const px = 150;
            book.mkt_price(px);

            n_asset = init_deposit / px * 2;
            book.sell(n_asset);

            THEN("market value of the book hasn't changed") {
                REQUIRE(book.mkt_value() == init_deposit);
            }

            THEN("the amount of cash hasn't changed") {
                REQUIRE(book.cash() == init_deposit);
            }

            THEN("the amount of assets hasn't changed") {
                REQUIRE(book.n_asset() == 0);
            }
        }
    }

    GIVEN("Some money and assets") {
        bt::price const init_deposit = 10000;
        bt::balance_book book(init_deposit);

        bt::price px = 250;
        book.mkt_price(px);

        int const init_assets = 10;
        book.buy(init_assets);

        int n_asset = 0;

        WHEN("selling some assets") {
            auto const cash_before = book.cash();
            n_asset = 3;
            book.sell(n_asset);

            THEN("the cash has increased") {
                REQUIRE(book.cash() == cash_before + n_asset * px);
            }

            THEN("the assets decreased by the amount sold") {
                REQUIRE(book.n_asset() == init_assets - n_asset);
            }

            THEN("the market value of the book hasn't changed") {
                REQUIRE(book.mkt_value() == init_deposit);
            }
        }

        WHEN("selling all of the assets") {
            auto const cash_before = book.cash();
            n_asset = init_assets;
            book.sell(n_asset);

            THEN("the market value of the book hasn't changed") {
                REQUIRE(book.mkt_value() == init_deposit);
            }

            THEN("the cash has increased") {
                REQUIRE(book.cash() == cash_before + n_asset * px);
            }

            THEN("the amount of assets is zero") {
                REQUIRE(book.n_asset() == 0);
            }
        }

        WHEN("selling more assets than held") {
            auto const cash_before = book.cash();
            auto const n_assets_on_margin = 5;
            n_asset = init_assets + n_assets_on_margin;
            book.sell(n_asset);

            THEN("the amount of assets is negative") {
                REQUIRE(book.n_asset() == -n_assets_on_margin);
            }

            THEN("the cash has increased") {
                REQUIRE(book.cash() == cash_before + n_asset * px);
            }

            THEN("the market value of the book hasn't changed") {
                REQUIRE(book.mkt_value() == init_deposit);
            }
        }

        WHEN("selling more assets than there's money and assets") {
            auto const cash_before = book.cash();
            auto const n_assets_before = book.n_asset();

            auto const n_assets_on_margin = (cash_before / px) * 20;

            n_asset = init_assets + n_assets_on_margin;
            book.sell(n_asset);

            THEN("the amount of assets hasn't changed") {
                REQUIRE(book.n_asset() == n_assets_before);
            }

            THEN("the cash hasn't changed") {
                REQUIRE(book.cash() == cash_before);
            }

            THEN("the market value of the book hasn't changed") {
                REQUIRE(book.mkt_value() == init_deposit);
            }
        }
    }
}
