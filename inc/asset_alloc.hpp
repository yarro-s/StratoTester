// Copyright 2020 Yarro S

#pragma once

#ifndef _HEAD_BLOCK
#include <iostream>
#endif

#include <unit_defs.hpp>
#include <utils.hpp>


namespace bt {

class asset_alloc {
 protected:
    virtual weight algo(price_t const &price_hist) = 0;

 public:
    virtual weight on_hist(price_t const &price_hist) {
        auto const w = algo(price_hist);

        // clamp to 0...1
        auto const w_hi = 1.0, w_lo = -1.0;
        return w >= w_lo && w <= w_hi ? w
                                      : (w > w_hi ? w_hi : w_lo);
    }

    virtual ~asset_alloc() {}
};
}   // namespace bt
