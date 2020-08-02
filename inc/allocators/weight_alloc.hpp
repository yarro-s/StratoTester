/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifndef _HEAD_BLOCK
#include <iostream>
#endif

#include <asset_alloc.hpp>
#include <utils.hpp>

namespace st {

class weight_alloc : public asset_alloc {
 private:
    weights const &weight_hist;

 public:
    weight algo(prices const &price_hist) noexcept {
        size_t idx_weight = (price_hist.size() - 1) % weight_hist.size();

        // std::cout << std::endl
        //           << ">>W PT: " << str_rep(price_hist) << " => "
        //           << weight_hist[idx_weight] << std::endl;
        return weight_hist[idx_weight];
    }

    explicit weight_alloc(weights const &weight_hist) noexcept
        : weight_hist(weight_hist) {}

    ~weight_alloc() {}
};
}   // namespace st

