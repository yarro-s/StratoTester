// Copyright 2020 Yarro S
#pragma once

#ifndef _HEAD_BLOCK
#include <memory>
#endif

#include <asset_alloc.hpp>
#include <strategies/lambda_alloc.hpp>
#include <backtest.hpp>


namespace bt {

class strategy : public asset_alloc {
 private:
    asset_alloc *alloc;
    backtest *model;

 protected:
    weight algo(price_t const &) override { return 0; }

 public:
    virtual strategy &rebalance_every(size_t m) {
        return *this;
    }

    virtual strategy &set_lookback(size_t n) {
        return *this;
    }

    strategy &set_alloc(asset_alloc *alloc) {
        this->alloc = alloc;
        return *this;
    }

    asset_alloc &get_alloc() {
        return *this->alloc;
    }

    strategy &set_model(backtest *model) {
        this->model = model;
        return *this;
    }

    backtest &get_model() {
        return *this->model;
    }

    explicit strategy(asset_alloc *alloc)
        : alloc(alloc) {}
};
}  // namespace bt
