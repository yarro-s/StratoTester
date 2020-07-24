/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <strategy.hpp>


namespace bt {

strategy *_mk_rebalance(asset_alloc *alloc, size_t m_rebalance);

strategy *_mk_lookback(asset_alloc *alloc, size_t n_lookback);

}  // namespace bt
