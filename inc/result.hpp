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

namespace st {

class result {
 private:
    weights wT;
    prices pvT;

 public:
    void reset() noexcept {
        wT.clear();
        pvT.clear();
    }

    void save(balance_book const &book) noexcept {
        wT.push_back(book.asset_value() / book.mkt_value());
        pvT.push_back(book.mkt_value());
    }

    weights const &asset_weights() const noexcept { return wT; }

    prices const &value_history() const noexcept { return pvT; }

    price total_return() const noexcept {
        return value_history().back() / value_history().front();
    }

    price max_drawdown() const noexcept;
};

class timed_result : public result {
 private:
    time_frame const t_frame;

 public:
    price cagr() const noexcept {
        auto const n_samples = value_history().size();
        float const n_years = n_samples * t_frame / tf::year;
        return pow(total_return(), 1.0 / n_years) - 1;
    }

    timed_result(result res, time_frame tf) noexcept
        : result(res), t_frame(tf) {}
};
}  // namespace st
