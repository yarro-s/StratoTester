/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifndef _HEAD_BLOCK
#include <cmath>
#endif

#include <balance_book.hpp>

namespace bt {

class result {
 private:
    weights wT;
    prices pvT;

 public:
    void reset() {
        wT.clear();
        pvT.clear();
    }

    void save(balance_book const &book) {
        wT.push_back(book.asset_value() / book.mkt_value());
        pvT.push_back(book.mkt_value());
    }

    weights const &wt() const {
        return wT;
    }

    prices const &pv() const {
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
