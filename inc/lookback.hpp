// Copyright 2020 Yarro S
#pragma once

#include <strategy.hpp>


namespace bt {

class lookback : public strategy {
 private:
    size_t n_lookback;

 public:
    weight on_hist(price_t const &price_hist) override {
        auto const len_passed = price_hist.size();
        weight wT = 0.0;

        if (len_passed >= n_lookback) {
            auto const t0 = price_hist.end() - n_lookback;
            auto const t_now = price_hist.end();

            std::cout << std::endl
                        << "  LOOKING BACK N = "
                        << std::distance(t0, t_now) << " ...";
            wT = get_alloc().on_hist(price_t(t0, t_now));
        }

        return wT;
    }

    strategy &rebalance_every(size_t m) override {
        return *this;
    }

    lookback(asset_alloc *alloc, size_t n_lookback)
        : n_lookback(n_lookback), strategy(alloc) {}
};
}  // namespace bt
