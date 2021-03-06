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

namespace st {
using algo_lambda =
    std::function<weight(prices const &)>;

class lambda_alloc : public asset_alloc {
 private:
    std::function<weight(prices)> const fa;

 public:
    virtual weight algo(prices const &price_hist) {
        return fa(price_hist);
    }

    explicit lambda_alloc(algo_lambda const &fa) : fa(fa) {}
};
}  // namespace st
