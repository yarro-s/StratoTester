/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifndef _HEAD_BLOCK
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#endif

#include <unit_defs.hpp>

namespace st {
template <typename T>
class t_series {
 protected:
    std::set<std::time_t> t_stamps;

    std::set<ticker> _tickers;
    std::unordered_map<ticker, std::vector<T>> vals;
    std::unordered_map<ticker, std::string> tags;

 public:
    std::vector<T> const &vals_for(ticker const &tckr) const {
        return vals.at(tckr);
    }

    std::set<std::time_t> const &timing() const noexcept {
        return t_stamps;
    }

    std::set<ticker> const &tickers() const noexcept {
        return _tickers;
    }

    size_t n_tickers() const noexcept {
        return _tickers.size();
    }

    size_t n_tpoints() const noexcept {
        return t_stamps.size();
    }

    void append_at(ticker tckr, std::time_t t_stamp, T val) noexcept {
        _tickers.insert(tckr);

        t_stamps.insert(t_stamp);

        vals[tckr].push_back(val);
    }
};
}  // namespace st
