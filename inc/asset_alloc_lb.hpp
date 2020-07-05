// Copyright 2020 Yarro S

#pragma once

#ifndef _HEAD_BLOCK
#include <iostream>
#endif

#include <asset_alloc.hpp>
#include <unit_defs.hpp>


namespace bt {

class asset_alloc_lb : public asset_alloc {
 private:
    asset_alloc &a_alloc;
    size_t n_lback;

 protected:
    weight algo(price_t const &price_hist) override {
        auto const len_passed = price_hist.size();
        weight wT = 0.0;

        if (len_passed >= n_lback) {
            auto const t0 = price_hist.end()-n_lback;
            auto const t_now = price_hist.end();

            // std::cout << std::endl
            //             << "  LOOKING BACK N = "
            //             << std::distance(t0, t_now) << " ...";
            wT = a_alloc.on_hist(price_t(t0, t_now));
        }

        return wT;
    }

 public:
    asset_alloc_lb(asset_alloc& a_alloc, size_t n)
        : a_alloc(a_alloc), n_lback(n) {}

    ~asset_alloc_lb() {}
};
}  // namespace bt
