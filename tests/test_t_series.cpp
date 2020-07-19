/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <csv.h>

#include <string>
#include <cmath>
#include <ctime>
#include <limits>
#include <sstream>

#include <catch2/catch.hpp>

#include <utils.hpp>
#include <t_series.hpp>

TEST_CASE("Time series initialization", "[t_series]") {
    std::string const datapath = "../tests/data/";
    std::string const time_fmt = "%Y-%m-%d";

    bt::t_series<bt::price> ts;

    struct tm timeinfo;

    SECTION("life-sized data loaded") {
        std::string const
            dataset_name = "^IXIC",
            index_name = "Date",
            field_name = "Close";

        io::CSVReader<2, io::trim_chars<>>
            in(datapath + dataset_name + ".csv");

        in.read_header(io::ignore_extra_column,
            index_name, field_name);

        std::string date;
        bt::price px;
        // timeinfo = (const struct tm){0};

        while (in.read_row(date, px)) {
            strptime(date.c_str(), time_fmt.c_str(), &timeinfo);
            timeinfo.tm_isdst = -1;
            auto const t_stamp = mktime(&timeinfo);

            ts.append_at(dataset_name, t_stamp, px);
        }

        REQUIRE(ts.vals_for(dataset_name).front() == Approx(100));
        REQUIRE(ts.vals_for(dataset_name).back() ==
                Approx(8650.139648));

        REQUIRE(ts.timing().size() == 12409);

        REQUIRE(ts.tickers().size()== 1);

        REQUIRE(ts.n_tickers() == 1);
        REQUIRE(ts.n_tpoints() == 12409);
    }

    SECTION("single-ticker time series initialized") {
        std::vector<std::string> t_stamps{
            "01/01/10", "02/01/10", "03/01/10", "04/01/10",
            "05/01/10"};
        bt::price_t vals{10.5, 12.3, 11.9, 14.7, 15.4};
        std::string tckr("A");

        // timeinfo = (const struct tm){0};
        for (auto v = vals.begin(); v != vals.end(); ++v) {
            auto const idxT = std::distance(vals.begin(), v);
            auto const t_raw = t_stamps[idxT];

            strptime(t_raw.c_str(), "%D", &timeinfo);
            timeinfo.tm_isdst = -1;
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

    SECTION("multi-ticker time series initialized") {
        io::CSVReader<4, io::trim_chars<>>
            in("../tests/data/csv_scratchbook.csv");
        in.read_header(io::ignore_extra_column,
                       "Date", "A", "T", "I");

        std::string date;
        bt::price A, T, I;
        // timeinfo = (const struct tm){0};

        while (in.read_row(date, A, T, I)) {
            strptime(date.c_str(), "%D", &timeinfo);
            timeinfo.tm_isdst = -1;
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
