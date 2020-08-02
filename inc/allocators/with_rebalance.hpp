/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <allocators/chained_alloc.hpp>


namespace st {

class with_rebalance : public chained_alloc {
 private:
    size_t n_rebalance;

 public:
    weight on_hist(prices const &price_hist) override {
        weight wT = algo(price_hist);

        if (!(price_hist.size() % n_rebalance)) {
            upd_model(price_hist.back(), wT);

            return wT;
        } else {
            return 0.0;
        }

        
    }

    with_rebalance(asset_alloc *next_alloc, size_t n_rebalance)
        : chained_alloc(next_alloc), n_rebalance(n_rebalance) {}
};
}  // namespace st
