/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _SRC_BLOCK
#include <rebalance.hpp>
#include <lookback.hpp>
#endif

namespace bt {

strategy *rebalance::set_lookback(size_t n_lookback) {
    return &set_alloc(new lookback(get_alloc(), n_lookback));
}
}  // namespace bt
