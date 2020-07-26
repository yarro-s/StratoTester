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

class rebalance : public strategy {
 private:
    size_t m_rebalance;

 public:
    weight on_hist(price_t const &price_hist) override {
        weight wT = 0.0;

        if (!(price_hist.size() % m_rebalance)) {
            wT = get_alloc()->on_hist(price_hist);

            upd_model(price_hist.back(), wT);
        }

        return wT;
    }

    strategy &rebalance_every(size_t m) override {
        m_rebalance = m;
        return *this;
    }

    strategy &set_lookback(size_t n) override {
        set_alloc(_mk_lookback(get_alloc(), n));
        return *this;
    }

    rebalance(asset_alloc *alloc, size_t m_rebalance)
        : strategy(alloc), m_rebalance(m_rebalance) {}
};
}  // namespace bt
