#include <iostream>
#include <sstream>
#include <unistd.h>
#include <BTLight.hpp>

#include <csv.h>

/*

class rep_ts
    : public bt::representer<bt::t_series<double>>
{
protected:
    std::string get_rep(
        bt::t_series<double> &ts) const override
    {
        std::stringstream rep;

        rep << "         ";

        for (auto const &tckr : ts.tickers)
        {
            rep << tckr << "   ";
        }
        rep << std::endl;

        for (auto t_stamp = ts.t_stamps.begin();
             t_stamp != ts.t_stamps.end(); ++t_stamp)
        {
            auto const idXT = std::distance(
                ts.t_stamps.begin(), t_stamp);

            rep << t_stamp_to_str(*t_stamp);

            for (auto const &tckr : ts.tickers)
            {
                rep << " " << ts.vals[tckr].at(idXT);
            }

            rep << std::endl;
        }
        return rep.str();
    }
public:
    rep_ts() {};
    ~rep_ts() {};

private:
    std::string t_stamp_to_str(
        std::time_t const &t_stamp) const
    {
        struct tm *timeinfo;
        size_t const n_t = 9;
        char buffer[9];

        timeinfo = localtime(&t_stamp);
        strftime(buffer, n_t, "%D", timeinfo);

        return std::string(buffer);
    }
};

*/

int main()
{
    char buff[FILENAME_MAX];
    getcwd(buff, FILENAME_MAX);
    std::cout << std::endl
              << "CWD: " << buff
              << std::endl
              << std::endl;

    io::CSVReader<4, io::trim_chars<>>
        in("csv_scratchbook.csv");
    in.read_header(io::ignore_extra_column,
                   "Date", "A", "T", "IBM");

    bt::t_series<bt::price> ts;

    std::string date;
    struct tm date_tm = (const struct tm){0};
    bt::price A, T, IBM;

    while (in.read_row(date, A, T, IBM))
    {
        strptime(date.c_str(), "%D", &date_tm);
        auto const t_stamp = mktime(&date_tm);

        ts.append_at("A", t_stamp, A);
        ts.append_at("T", t_stamp, T);
        ts.append_at("IBM", t_stamp, IBM);
    }

    std::cout << std::endl
              << "TS: " << std::endl
              << bt::str_rep(ts) << std::endl;

    return 0;
}