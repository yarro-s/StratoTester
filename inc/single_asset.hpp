/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <backtest.hpp>
#include <asset_alloc.hpp>
#include <balance_book.hpp>
#include <result.hpp>


namespace bt {

class single_asset : public backtest {
 private:
    result res;
    balance_book book;
    asset_alloc &a_alloc;

    weight _last_w;

    void reset() {
        book.reset();
        res.reset();
        _last_w = 0.0;
    }

 public:
    result results() {
        return res;
    }

    timed_result results(time_frame tf) {
        return timed_result(res, tf);
    }

    single_asset &update(price px, weight w);
    single_asset &run(price_ts const &pT);

    explicit single_asset(asset_alloc &a_alloc)
        : single_asset(a_alloc, 1.0e12) {}

    single_asset(asset_alloc &a_alloc, price initial_deposit)
        : book(initial_deposit), a_alloc(a_alloc) {
        reset();
        a_alloc.set_model(this);
    }
};
}  // namespace bt
