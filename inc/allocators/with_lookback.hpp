/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <allocators/chained_alloc.hpp>


namespace st {

class with_lookback : public chained_alloc {
 private:
    size_t m_lookback;

 protected:
    weight algo(prices const &price_hist) override {
        auto const t0 = price_hist.end() - m_lookback;
        auto const t_now = price_hist.end();

        return chained_alloc::algo(prices(t0, t_now));
    }

 public:
    weight on_hist(prices const &price_hist) override {
        auto const len_passed = price_hist.size();
        weight wT = 0.0;

        if (len_passed >= m_lookback) {
            wT = algo(price_hist);
            upd_model(price_hist.back(), wT);
        }

        return wT;
    }

    with_lookback(asset_alloc *next_alloc, size_t m_lookback)
        : chained_alloc(next_alloc), m_lookback(m_lookback) {}
};
}  // namespace st
