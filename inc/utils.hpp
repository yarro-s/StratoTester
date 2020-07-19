/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifndef _HEAD_BLOCK
#include <string>
#include <iostream>
#include <sstream>
#endif

#include <t_series.hpp>
#include <balance_book.hpp>
#include <unit_defs.hpp>


namespace bt {
std::string str_rep(balance_book const &book);

std::string str_rep(t_series<double> const &ts);

std::string str_rep(price_t const &sT);
}  // namespace bt
