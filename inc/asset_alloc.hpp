/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifndef _HEAD_BLOCK
#include <iostream>
#endif

#include <unit_defs.hpp>
#include <utils.hpp>
#include <backtest.hpp>


namespace bt {

class asset_alloc {
 private:
    backtest *model = nullptr;

 protected:
    virtual weight algo(prices const &price_hist) = 0;

    // TODO(yarrik): convert to vararg
    backtest &upd_model(price px, weight w) {
        if (model)
            return this->model->update(px, w);
        else
            return *this->model;
    }

 public:
    asset_alloc &set_model(backtest *model) {
        this->model = model;
        return *this;
    }

    virtual weight on_hist(prices const &price_hist) {
        auto w = algo(price_hist);

        // clamp to 0...1
        auto const w_hi = 1.0, w_lo = -1.0;
        w = w >= w_lo && w <= w_hi ? w
                                      : (w > w_hi ? w_hi : w_lo);
        upd_model(price_hist.back(), w);
        return w;
    }

    virtual ~asset_alloc() {}
};
}   // namespace bt
