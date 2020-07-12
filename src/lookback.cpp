// Copyright 2020 Yarro S

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
