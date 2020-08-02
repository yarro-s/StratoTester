/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifndef _HEAD_BLOCK
#include <vector>
#include <chrono>
#include <string>
#endif


namespace bt {
using price = double;
using weight = double;
using volume = long long;

using prices = std::vector<price>;
using weights = std::vector<weight>;

using ticker = std::string;

using time_frame = std::chrono::duration<float>;

namespace tf {
    auto constexpr second = time_frame(1.0);
    auto constexpr minute = 60 * second;
    auto constexpr hour = 60 * minute;
    auto constexpr day = 24 * hour;
    auto constexpr week = 7 * day;
    auto constexpr month = 30.42 * day;
    auto constexpr year = 52 * week;
}  // namespace tf
}  // namespace bt
