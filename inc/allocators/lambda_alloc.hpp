// Copyright 2020 Yarro S

#pragma once

#ifndef _HEAD_BLOCK
#include <functional>
#endif

#include <asset_alloc.hpp>

namespace bt {
using algo_lambda =
    std::function<weight(price_t const &)>;

class lambda_alloc : public asset_alloc {
 private:
    std::function<weight(price_t)> const fa;

 public:
    virtual weight algo(price_t const &price_hist) {
        return fa(price_hist);
    }

    lambda_alloc()
        : fa([](price_t const &) {
            // std::cout << "DEFAULT ALGO LAMBDA!" << std::endl;
            return 0.0; }) {}

    explicit lambda_alloc(algo_lambda fa) : fa(fa) {}
};
}  // namespace bt
