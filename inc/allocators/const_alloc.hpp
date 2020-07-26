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

#include <asset_alloc.hpp>
#include <unit_defs.hpp>


namespace bt {

class const_alloc : public asset_alloc {
 private:
    bool has_updated = false;

    weight const weight_;

 protected:
    weight algo(price_t const &) override {
        return weight_;
    }

    weight on_hist(price_t const &price_hist) override {
        auto const wT = algo(price_hist);

        if (!has_updated) {  // call once
            upd_model(price_hist.back(), wT);

            has_updated = true;
        }
        return wT;
    }

 public:
    explicit const_alloc(weight w) noexcept
        : weight_(w) {}

    ~const_alloc() {}
};
}   // namespace bt

