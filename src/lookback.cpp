/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _SRC_BLOCK
#include <lookback.hpp>
#include <rebalance.hpp>
#endif

namespace bt {

strategy *lookback::rebalance_every(size_t m_rebalance) {
    auto strat_rb = new rebalance(get_alloc(), m_rebalance);
    strat_rb->set_alloc(this);

    return strat_rb;
}
}  // namespace bt
