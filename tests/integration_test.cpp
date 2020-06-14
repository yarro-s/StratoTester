#include <iostream>
#include <ctime>
#include <unistd.h>
#include <catch2/catch.hpp>
#include <csv.h>
#include <BTLight.hpp>

TEST_CASE("Backtest complete workflow", "[backtest]")
{
    bt::price_t pt;

    size_t n_lookback = 3;

    auto const sma_x_p = 
    [](bt::price_t const &pT)
    {
        auto const sma = 
            std::accumulate(pT.begin(), pT.end(), 0) 
                / pT.size();

        return pT.back() > sma ? 1.0 : -1.0;
    };

    SECTION("short price data is loaded and processed")
    {
        // std::cout << std::endl
        //           << "---\nINTEGRATION "
        //           << std::endl;
        n_lookback = 3;
        std::string dataset_name = "sma_model",
                    field_name = "Close";

        io::CSVReader<1, io::trim_chars<>>
            in("../tests/data/" + dataset_name + ".csv");

        in.read_header(io::ignore_extra_column, field_name);

        bt::price p;
        size_t n_expected = 0;

        while (in.read_row(p)) 
        {
            pt.push_back(p);
            n_expected++;
        }

        CHECK(pt.size() == n_expected);

        auto back_test = bt::single_asset(pt, 100);
        auto sma = bt::asset_alloc_lb(n_lookback, sma_x_p);
        auto const &res = back_test.run(sma).results();

        // TO DO: cross-check with Python bt
        auto const growth_expected = 0.584; 
        CHECK(res.growth() == Approx(growth_expected));
   }

    SECTION("prices are loaded and processed")
    {
        std::string dataset_name = "^IXIC",
                    field_name = "Close";

        io::CSVReader<1, io::trim_chars<>>
            in("../tests/data/" + dataset_name + ".csv");

        in.read_header(io::ignore_extra_column, field_name);

        bt::price p;
        size_t n_expected = 0;

        while (in.read_row(p)) 
        {
            pt.push_back(p);
            n_expected++;
        }

        CHECK(pt.size() == n_expected);

        auto back_test = bt::single_asset(pt);
        auto sma = bt::asset_alloc_lb(n_lookback, sma_x_p);
        auto const &res = back_test.run(sma).results();

        auto const growth_expected = 85.8489427978; 
        CHECK(res.growth() == Approx(growth_expected));  
   } 
}