// Copyright 2020 Yarro S

#pragma once

#ifndef _HEAD_BLOCK
#include <iostream>
#endif

#include <asset_alloc.hpp>
#include <unit_defs.hpp>
#include <utils.hpp>


namespace bt {

class asset_alloc_rb : public asset_alloc {
 private:
    asset_alloc &a_alloc;
    size_t m_rbalance;

 protected:
    weight algo(price_t const &price_hist) override {
        weight wT = 0.0;

        if (!(price_hist.size() % m_rbalance)) {
            // std::cout << std::endl
            //             << "  REBALANCING M = "
            //             << price_hist.size() << " ...";
            wT = a_alloc.on_hist(price_hist);
        }

        return wT;
    }

 public:
    asset_alloc_rb(asset_alloc &a_alloc, size_t m)
        : a_alloc(a_alloc), m_rbalance(m) {}

    ~asset_alloc_rb() {}
};
}  // namespace bt
