#include <string>
#include <math.h>
#include <limits>
#include <catch2/catch.hpp>
#include <sstream>
#include <ctime>
#include <csv.h>
#include <utils.hpp>
#include <t_series.hpp>

TEST_CASE("Time series initialization", "[t_series]")
{
    bt::t_series<bt::price> ts;

    struct tm timeinfo = (const struct tm){0};

    SECTION("life-sized data loaded") 
    {
        std::string dataset_name = "^IXIC",
                    index_name = "Date",
                    field_name = "Close";

        io::CSVReader<2, io::trim_chars<>>
            in("../tests/data/" + dataset_name + ".csv");

        in.read_header(io::ignore_extra_column,
                       index_name, field_name);

        std::string date;
        bt::price p;
        size_t n = 0;

        while (in.read_row(date, p))
        { 
            strptime(date.c_str(), "%Y-%m-%d", &timeinfo);
            auto const t_stamp = mktime(&timeinfo);

            ts.append_at(dataset_name, t_stamp, p);
        }

        REQUIRE(ts.vals_for(dataset_name).front() ==
                Approx(100));
        REQUIRE(ts.vals_for(dataset_name).back() ==
                Approx(8650.139648));

        REQUIRE(ts.timing().size() == 12409);

        REQUIRE(ts.tickers().size()== 1);

        REQUIRE(ts.n_tickers() == 1);
        REQUIRE(ts.n_tpoints() == 12409);
    }

    SECTION("single-ticker time series initialized")
    {
        std::vector<std::string> t_stamps{
            "01/01/10", "02/01/10", "03/01/10", "04/01/10",
            "05/01/10"};
        bt::price_t vals{10.5, 12.3, 11.9, 14.7, 15.4};
        std::string tckr("A");

        for (auto v = vals.begin(); v != vals.end(); ++v)
        {
            auto const idxT = std::distance(vals.begin(), v);
            auto const t_raw = t_stamps[idxT];

            strptime(t_raw.c_str(), "%D", &timeinfo);
            auto const t_stamp = mktime(&timeinfo);

            ts.append_at(tckr, t_stamp, *v);
        }

        REQUIRE(ts.vals_for(tckr).front() == vals.front());
        REQUIRE(ts.vals_for(tckr).back() == vals.back());

        REQUIRE(ts.timing().size() == t_stamps.size());

        REQUIRE(ts.tickers().size()== 1);

        REQUIRE(ts.n_tickers() == 1);
        REQUIRE(ts.n_tpoints() == t_stamps.size());
    }

    SECTION("multi-ticker time series initialized")
    {
        io::CSVReader<4, io::trim_chars<>>
            in("../tests/data/csv_scratchbook.csv");
        in.read_header(io::ignore_extra_column,
                       "Date", "A", "T", "I");

        std::string date;
        bt::price A, T, I;

        while (in.read_row(date, A, T, I))
        {
            strptime(date.c_str(), "%D", &timeinfo);
            auto const t_stamp = mktime(&timeinfo);

            ts.append_at("A", t_stamp, A);
            ts.append_at("T", t_stamp, T);
            ts.append_at("I", t_stamp, I);
        }

        // see csv_scratchbook.csv for the expected values
        REQUIRE(ts.vals_for("A").front() == Approx(5.42));
        REQUIRE(ts.vals_for("I").back() == Approx(243.5));

        REQUIRE(ts.timing().size() == 5);

        REQUIRE(ts.tickers().size()== 3);

        REQUIRE(ts.n_tickers() == 3);
        REQUIRE(ts.n_tpoints() == 5);
    }
}