// Copyright 2020 Yarro S

#pragma once

#ifndef _HEAD_BLOCK
#include <deps.h>
#endif

#include <balance_book.hpp>
#include <unit_defs.hpp>  /// --


namespace bt {

class result {
 private:
    weight_t wT;
    price_t pvT;

 public:
    void save(balance_book book) {
        wT.push_back(book.asset_value() / book.mkt_value());
        pvT.push_back(book.mkt_value());
    }

    weight_t const &wt() const {
        return wT;
    }

    price_t const &pv() const {
        return pvT;
    }

    price growth() const {
        return pv().back() / pv().front();
    }

    price max_drawdown() const;
};

class timed_result : public result {
 private:
    time_frame const t_frame;

 public:
    price cagr() const {
        auto const n_samples = pv().size();
        float const n_years = n_samples * t_frame /
                                tf::year;
        return pow(growth(), 1.0 / n_years) - 1;
    }

    timed_result(result res, time_frame tf)
        : result(res), t_frame(tf) {}
};
}  // namespace bt
