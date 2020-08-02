/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifndef _HEAD_BLOCK
#include <functional>
#endif

#include <asset_alloc.hpp>

namespace bt {
using algo_lambda =
    std::function<weight(price_ts const &)>;

class lambda_alloc : public asset_alloc {
 private:
    std::function<weight(price_ts)> const fa;

 public:
    virtual weight algo(price_ts const &price_hist) {
        return fa(price_hist);
    }

    lambda_alloc()
        : fa([](price_ts const &) {
            // std::cout << "DEFAULT ALGO LAMBDA!" << std::endl;
            return 0.0; }) {}

    explicit lambda_alloc(algo_lambda fa) : fa(fa) {}
};
}  // namespace bt
