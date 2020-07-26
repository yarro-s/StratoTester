/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <strategy.hpp>
#include <mk_strategy.hpp>


namespace bt {

class lookback : public strategy {
 private:
    size_t n_lookback;

 public:
    weight on_hist(price_t const &price_hist) override {
        auto const len_passed = price_hist.size();
        weight wT = 0.0;

        if (len_passed >= n_lookback) {
            auto const t0 = price_hist.end() - n_lookback;
            auto const t_now = price_hist.end();

            wT = get_alloc()->on_hist(price_t(t0, t_now));

            upd_model(price_hist.back(), wT);
        }

        return wT;
    }

    strategy &rebalance_every(size_t m) override {
        auto strat_rb = _mk_rebalance(get_alloc(), m);
        strat_rb->set_alloc(this);

        return *strat_rb;
    }

    strategy &set_lookback(size_t n) override {
        n_lookback = n;
        return *this;
    }

    lookback(asset_alloc *alloc, size_t n_lookback)
        : strategy(alloc), n_lookback(n_lookback) {}

 public:
    template <class Alloc, class... Args>
    static lookback *with(const Args&... args) {
        auto allocator =  new Alloc(args...);
        return new bt::lookback(allocator, 0);
    }
};
}  // namespace bt
