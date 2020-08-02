/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#ifndef _HEAD_BLOCK
#include <cmath>
#endif

#include <unit_defs.hpp>


namespace st {

class balance_book {
 private:
    price cash_;
    int n_asset_;
    price curr_mkt_price;

    price _init_depo;

 public:
    void reset() {
        cash_ = _init_depo;
        n_asset_ = 0;
        curr_mkt_price = 0;
    }

    price mkt_price() const { return curr_mkt_price; }

    balance_book &mkt_price(price px) {
        curr_mkt_price = px;
        return *this;
    }

    price cash() const noexcept { return cash_; }

    int n_asset() const noexcept { return n_asset_; }

    price asset_value() const noexcept { return n_asset() * mkt_price(); }

    price mkt_value() const noexcept { return cash() + asset_value(); }

    balance_book &sell(size_t amount) noexcept {
        auto const vol = -(static_cast<int>(amount) * mkt_price());

        if (static_cast<int>(amount) <= n_asset()
            || abs(vol) <= cash()) {
            take_at(-static_cast<int>(amount), vol);
        }
        return *this;
    }

    balance_book &buy(size_t amount) noexcept {
        auto const vol = amount * mkt_price();

        if (vol <= cash()) {
            take_at(amount, vol);
        }
        return *this;
    }

    explicit balance_book(price init_depo) noexcept
        : _init_depo(init_depo) {
        reset();
    }

 private:
    void take_at(int amount, price vol) noexcept {
        n_asset_ += amount;
        cash_ -= vol;
    }
};
}  // namespace st
