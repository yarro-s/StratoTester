// Copyright 2020 Yarro S
#pragma once

#include <strategy.hpp>


namespace bt {

class rebalance : public strategy {
 private:
    size_t m_rebalance;

 public:
    weight on_hist(price_t const &price_hist) override {
        weight wT = 0.0;

        if (!(price_hist.size() % m_rebalance)) {
            // std::cout << std::endl
            //             << "  REBALANCING M = "
            //             << price_hist.size() << " ...";
            wT = get_alloc().on_hist(price_hist);
        }

        return wT;
    }

    strategy &set_lookback(size_t n) override {
        return *this;
    }

    rebalance(asset_alloc *alloc, size_t m_rebalance)
        : m_rebalance(m_rebalance), strategy(alloc) {}
};
}  // namespace bt
