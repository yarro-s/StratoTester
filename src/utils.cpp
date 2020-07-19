// Copyright 2020 Yarro S

#ifndef _SRC_BLOCK
#include <utils.hpp>
#endif

namespace bt {
std::string str_rep(balance_book const &book) {
    std::stringstream rep;

    rep << book.cash() << " & " << book.n_asset();

    return rep.str();
}

std::string str_rep(t_series<double> const &ts) {
    std::stringstream rep;

    auto const t_stamp_to_str =
    [](std::time_t const &t_stamp) {
        struct tm *timeinfo;
        size_t const n_t = 9;
        char buffer[9];

        timeinfo = localtime(&t_stamp);
        strftime(buffer, n_t, "%D", timeinfo);

        return std::string(buffer);
    };

    rep << "         ";

    for (auto const &tckr : ts.tickers()) {
        rep << tckr << "   ";
    }
    rep << std::endl;

    for (auto t_stamp = ts.timing().begin();
            t_stamp != ts.timing().end(); ++t_stamp) {
        auto const idXT = std::distance(
            ts.timing().begin(), t_stamp);

        rep << t_stamp_to_str(*t_stamp);

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
