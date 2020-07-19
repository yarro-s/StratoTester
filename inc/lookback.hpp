/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <strategy.hpp>
#include <rebalance.hpp>


namespace bt {

class lookback : public strategy {
 private:
    size_t n_lookback;

 public:
    weight on_hist(price_t const &price_hist) override {
        auto const len_passed = price_hist.size();
        weight wT = 0.0;

        // std::cout << std::endl
        //           << "  LOOKING BACK N = "
        //           << n_lookback << " ... " << len_passed;

        if (len_passed >= n_lookback) {
            auto const t0 = price_hist.end() - n_lookback;
            auto const t_now = price_hist.end();

            wT = get_alloc()->on_hist(price_t(t0, t_now));

            upd_model(price_hist.back(), wT);
        }

        return wT;
    }

    strategy *set_lookback(size_t n) override {
        n_lookback = n;
        return this;
    }

    strategy *rebalance_every(size_t m_rebalance) override;

    lookback(asset_alloc *alloc, size_t n_lookback)
        : strategy(alloc), n_lookback(n_lookback) {}
};
}  // namespace bt
