/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <asset_alloc.hpp>


namespace bt {

class chained_alloc : public asset_alloc {
 private:
    asset_alloc *next_alloc;

 protected:
    weight algo(price_t const &price_hist) override {
        return next_alloc->on_hist(price_hist);
    }

 public:
    asset_alloc &set_next(asset_alloc *next_alloc) {
        this->next_alloc = next_alloc;
        return *this;
    }

    asset_alloc *get_next() {
        return this->next_alloc;
    }

    weight on_hist(price_t const &price_hist) override {
        weight wT = algo(price_hist);
        upd_model(price_hist.back(), wT);

        return wT;
    }

    explicit chained_alloc(asset_alloc *alloc)
        : next_alloc(alloc) {}
};
}  // namespace bt
