// Copyright 2020 Yarro S
#pragma once

#ifndef _HEAD_BLOCK
#include <memory>
#endif

#include <asset_alloc.hpp>
#include <backtest.hpp>


namespace bt {

class strategy : public asset_alloc {
 private:
    asset_alloc *alloc;

 protected:
    weight algo(price_t const &) override { return 0; }

 public:
    weight on_hist(price_t const &price_hist) override = 0;

    virtual strategy *rebalance_every(size_t m) = 0;

    virtual strategy *set_lookback(size_t n) = 0;

    strategy &set_alloc(asset_alloc *alloc) {
        this->alloc = alloc;
        return *this;
    }

    asset_alloc *get_alloc() {
        return this->alloc;
    }

    explicit strategy(asset_alloc *alloc)
        : alloc(alloc) {}
};
}  // namespace bt
