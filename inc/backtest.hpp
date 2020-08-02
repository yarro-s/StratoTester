/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <unit_defs.hpp>


namespace bt {
class backtest {
 public:
    virtual backtest &update(price px, weight w) = 0;
    virtual backtest &run(prices const &pT) = 0;
};
}  // namespace bt
