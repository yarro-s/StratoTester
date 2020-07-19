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

namespace bt {

class weight_alloc : public asset_alloc {
 private:
    weight_t const &weights;

 public:
    weight algo(price_t const &prices) noexcept {
        size_t idx_weight = (prices.size() - 1) % weights.size();

        // std::cout << std::endl
        //           << ">>W PT: " << str_rep(prices) << " => "
        //           << weights[idx_weight] << std::endl;
        return weights[idx_weight];
    }

    explicit weight_alloc(weight_t const &weights) noexcept
        : weights(weights) {}

    ~weight_alloc() {}
};
}   // namespace bt

