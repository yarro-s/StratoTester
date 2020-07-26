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

#include <allocators/chained_alloc.hpp>
#include <allocators/with_lookback.hpp>
#include <allocators/with_rebalance.hpp>


namespace bt {

class strategy : public chained_alloc {
 private:
    bool rebalancing_set = false;

 public:
    virtual strategy &look_back(size_t m) {
        if (rebalancing_set) {  // rebalancing must run first
            auto my_rebalanced_alloc =
                static_cast<chained_alloc*>(get_next());
            auto lookback_alloc =
                new with_lookback(my_rebalanced_alloc->get_next(), m);
            my_rebalanced_alloc->set_next(lookback_alloc);
        } else {
            set_next(new with_lookback(get_next(), m));
        }
        return *this;
    }

    virtual strategy &rebalance_every(size_t n) {
        set_next(new with_rebalance(get_next(), n));
        rebalancing_set = true;
        return *this;
    }

    explicit strategy(asset_alloc *alloc)
        : chained_alloc(alloc) {}
};
}  // namespace bt
