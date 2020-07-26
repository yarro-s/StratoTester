/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <allocators/chained_alloc.hpp>


namespace bt {

class with_rebalance : public chained_alloc {
 private:
    size_t n_rebalance;

 public:
    weight on_hist(price_t const &price_hist) override {
        weight wT = 0.0;

        if (!(price_hist.size() % n_rebalance)) {
            wT = algo(price_hist);
            upd_model(price_hist.back(), wT);
        }

        return wT;
    }

    with_rebalance(asset_alloc *next_alloc, size_t n_rebalance)
        : chained_alloc(next_alloc), n_rebalance(n_rebalance) {}
};
}  // namespace bt
