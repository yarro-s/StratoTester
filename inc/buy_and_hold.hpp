// Copyright 2020 Yarro S
#pragma once

#include <strategy.hpp>
#include <strategies/const_alloc.hpp>


namespace bt {

class buy_and_hold : public strategy {
 private:
    bool has_updated = false;

 public:
    weight on_hist(price_t const &price_hist) override {
        auto const wT = get_alloc()->on_hist(price_hist);

        if (!has_updated) {  // call once
            upd_model(price_hist.back(), wT);

            has_updated = true;
        }
        return wT;
    }

    strategy *rebalance_every(size_t) override {
        return this;
    }

    strategy *set_lookback(size_t) override {
        return this;
    }

    buy_and_hold(weight const w)
        : strategy(new const_alloc(w)) {}
};
}  // namespace bt