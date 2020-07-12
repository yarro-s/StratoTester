// Copyright 2020 Yarro S

#ifndef _SRC_BLOCK
#include <rebalance.hpp>
#include <lookback.hpp>
#endif

namespace bt {

strategy *rebalance::set_lookback(size_t n_lookback) {
    return &set_alloc(new lookback(get_alloc(), n_lookback));
}
}  // namespace bt
