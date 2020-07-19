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
#include <unit_defs.hpp>


namespace bt {

class const_alloc : public asset_alloc {
 private:
    weight const weight_;

 protected:
    weight algo(price_t const &) noexcept {
        return weight_;
    }

 public:
    explicit const_alloc(weight w) noexcept
        : weight_(w) {}

    ~const_alloc() {}
};
}   // namespace bt

