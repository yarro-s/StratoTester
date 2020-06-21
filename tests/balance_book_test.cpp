
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
        book.buy_at(10, 55.3);

        auto const book_expected =
            "447 & 10";
        REQUIRE(book_expected == bt::str_rep(book));
    }

    SECTION("trades are accounted for")
    {
        book.buy_at(11, 50.0);
        book.buy_at(5, 60.0);

        auto const book_expected_1 =
            "150 & 16";
        REQUIRE(book_expected_1 == bt::str_rep(book));
    }

    SECTION("asset appreciation is accounted for")
    {
        book.buy_at(5, 100.0);
        book.buy_at(3, 85.0);

        bt::price curr_px = 120.0;

        auto const mkt_val_expected =
            (5 + 3) * curr_px + (1000 - (5*100.0 + 3*85.0));

        REQUIRE(mkt_val_expected ==
                book.mkt_value_at(curr_px));
    }

    SECTION("full divesting is done")
    {
        book.buy_at(6, 80.5);
        book.buy_at(2, 65.3);

        auto cash_expected =
            1000.0 - (6 * 80.5 + 2 * 65.3);

        REQUIRE(cash_expected == book.cash());

        bt::price curr_px = 100.0;

        book.sell_at(3, curr_px);

        curr_px = 110.0;

        book.sell_at(5, curr_px);

        cash_expected += 3 * 100.0 + 5 * 110.0;

        REQUIRE(cash_expected == book.cash());
    }

    SECTION("partial divesting is done")
    {
        book.buy_at(10, 25);
        book.buy_at(5, 20.4);
        book.buy_at(8, 30.5);

        auto cash_expected =
            1000.0 - 10 * 25 - 5 * 20.4 - 8 * 30.5;
        REQUIRE(cash_expected == book.cash());

        auto const curr_px = 45.5;
        book.sell_at(12, curr_px);

        cash_expected += 12 * curr_px;
        REQUIRE(cash_expected == book.cash());

        auto const mkt_val_expected =
            11 * curr_px + cash_expected;
        REQUIRE(mkt_val_expected ==
                book.mkt_value_at(curr_px));
    }

    SECTION("short trading")
    {
        book.sell_at(5, 15);
        book.sell_at(2, 20);

        auto cash_expected =
            1000.0 + 5 * 15 + 2 * 20;
        REQUIRE(cash_expected == book.cash());

        book.buy_at(4, 8);
        cash_expected -= 4 * 8; 
        REQUIRE(cash_expected == book.cash());

        book.buy_at(3, 50);
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
        book.buy_at(10, 14.6);

        REQUIRE(0.0 == book.cash());
        REQUIRE(0 == book.n_asset());
    }

    SECTION("no trading on margin")
    {
        bt::balance_book book(100.0);
        book.buy_at(5, 12.0);

        auto const cash_expected = 100.0 - 5 * 12.0;
        REQUIRE(cash_expected == book.cash());

        book.buy_at(10, 25.4);
        REQUIRE(cash_expected == book.cash());

        auto const n_asset_expected = 5;
        REQUIRE(n_asset_expected == book.n_asset());
    }

    SECTION("no short trading on margin")
    {
        bt::balance_book book(100.0);
        book.sell_at(5, 12.0);

        auto const cash_expected = 100.0 + 5 * 12.0;
        REQUIRE(cash_expected == book.cash());

        book.sell_at(20, 25.4);
        REQUIRE(cash_expected == book.cash());

        auto const n_asset_expected = -5;
        REQUIRE(n_asset_expected == book.n_asset());
    }
}
