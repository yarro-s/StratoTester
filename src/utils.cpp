/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _SRC_BLOCK
#include <iomanip>

#include <utils.hpp>
#endif

#define BT_N_TIME_STRLEN 20


namespace bt {

std::time_t str_to_time(std::string const &time_str,
                        std::string const &time_fmt) {
    std::tm tm = std::tm();

    std::istringstream ss(time_str);
    ss >> std::get_time(&tm, time_fmt.c_str());

    return mktime(&tm);
}

std::string str_rep(time_t const &t_stamp, std::string const &time_fmt) {
    std::tm tm = std::tm();
    localtime_r(&t_stamp, &tm);

    char buff[BT_N_TIME_STRLEN]{0};

    strftime(buff, BT_N_TIME_STRLEN, time_fmt.c_str(), &tm);
    return std::string(buff);
}

std::string str_rep(balance_book const &book) {
    std::stringstream rep;

    rep << book.cash() << " & " << book.n_asset();

    return rep.str();
}

std::string str_rep(t_series<double> const &ts) {
    std::stringstream rep;

    rep << "         ";

    for (auto const &tckr : ts.tickers()) {
        rep << tckr << "   ";
    }
    rep << std::endl;

    for (auto t_stamp = ts.timing().begin();
            t_stamp != ts.timing().end(); ++t_stamp) {
        auto const idXT = std::distance(
            ts.timing().begin(), t_stamp);

        rep << str_rep(*t_stamp);

        for (auto const &tckr : ts.tickers()) {
            rep << " " << ts.vals_for(tckr).at(idXT);
        }

        rep << std::endl;
    }
    return rep.str();
}

std::string str_rep(price_t const &sT) {
    std::ostringstream ss;
    ss << '[';
    bool first = true;
    for (price const &elem : sT) {
        if (!first) {
            ss << ", ";
        }
        ss << elem;
        first = false;
    }
    ss << ']';
    return ss.str();
}
}  // namespace bt
