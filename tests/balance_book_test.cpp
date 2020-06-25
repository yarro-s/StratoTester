
#include <sstream>
#include <catch2/catch.hpp>
#include <balance_book.hpp>
#include <utils.hpp>

TEST_CASE("Balance book creation and transaction",
          "[balance_book]")
{
    bt::balance_book book(1000.0);

    SECTION("balance book is initialized")
    {
        auto const book_expected = "1000 & 0";
        REQUIRE(book_expected == bt::str_rep(book));
    }

    SECTION("trade is accounted for")
    {
        book.mkt_price(55.3).buy(10);

        auto const book_expected =
            "447 & 10";
        REQUIRE(book_expected == bt::str_rep(book));
    }

    SECTION("trades are accounted for")
    {
        book.mkt_price(50.0).buy(11);
        book.mkt_price(60.0).buy(5);

        auto const book_expected_1 =
            "150 & 16";
        REQUIRE(book_expected_1 == bt::str_rep(book));
    }

    SECTION("asset appreciation is accounted for")
    {
        book.mkt_price(100.0).buy(5);
        book.mkt_price(85.0).buy(3);

        bt::price curr_px = 120.0;

        auto const mkt_val_expected =
            (5 + 3) * curr_px + (1000 - (5*100.0 + 3*85.0));

        REQUIRE(mkt_val_expected ==
                book.mkt_price(curr_px).mkt_value());
    }

    SECTION("full divesting is done")
    {
        book.mkt_price(80.5).buy(6);
        book.mkt_price(65.3).buy(2);

        auto cash_expected =
            1000.0 - (6 * 80.5 + 2 * 65.3);

        REQUIRE(cash_expected == book.cash());

        bt::price curr_px = 100.0;

        book.mkt_price(curr_px).sell(3);

        curr_px = 110.0;

        book.mkt_price(curr_px).sell(5);

        cash_expected += 3 * 100.0 + 5 * 110.0;

        REQUIRE(cash_expected == book.cash());
    }

    SECTION("partial divesting is done")
    {
        book.mkt_price(25).buy(10);
        book.mkt_price(20.4).buy(5);
        book.mkt_price(30.5).buy(8);

        auto cash_expected =
            1000.0 - 10 * 25 - 5 * 20.4 - 8 * 30.5;
        REQUIRE(cash_expected == book.cash());

        auto const curr_px = 45.5;
        book.mkt_price(curr_px).sell(12);

        cash_expected += 12 * curr_px;
        REQUIRE(cash_expected == book.cash());

        auto const mkt_val_expected =
            11 * curr_px + cash_expected;
        REQUIRE(mkt_val_expected ==
                book.mkt_value());
    }

    SECTION("short trading")
    {
        book.mkt_price(15).sell(5);
        book.mkt_price(20).sell(2);

        auto cash_expected =
            1000.0 + 5 * 15 + 2 * 20;
        REQUIRE(cash_expected == book.cash());

        book.mkt_price(8).buy(4);
        cash_expected -= 4 * 8; 
        REQUIRE(cash_expected == book.cash());

        book.mkt_price(50).buy(3);
        cash_expected -= 3 * 50; 
        REQUIRE(cash_expected == book.cash());
        REQUIRE(0 == book.n_asset());
    }
}


TEST_CASE("Balance book marginal cases",
          "[balance_book]")
{
    SECTION("no zero-balance trading")
    {
        bt::balance_book book(0.0);
        book.mkt_price(14.6).buy(10);

        REQUIRE(0.0 == book.cash());
        REQUIRE(0 == book.n_asset());
    }

    SECTION("no trading on margin")
    {
        bt::balance_book book(100.0);
        book.mkt_price(12.0).buy(5);

        auto const cash_expected = 100.0 - 5 * 12.0;
        REQUIRE(cash_expected == book.cash());

        book.mkt_price(25.4).buy(10);
        REQUIRE(cash_expected == book.cash());

        auto const n_asset_expected = 5;
        REQUIRE(n_asset_expected == book.n_asset());
    }

    SECTION("no short trading on margin")
    {
        bt::balance_book book(100.0);
        book.mkt_price(12.0).sell(5);

        auto const cash_expected = 100.0 + 5 * 12.0;
        REQUIRE(cash_expected == book.cash());

        book.mkt_price(25.4).sell(20);
        REQUIRE(cash_expected == book.cash());

        auto const n_asset_expected = -5;
        REQUIRE(n_asset_expected == book.n_asset());
    }
}
