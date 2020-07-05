// Copyright 2020 Yarro S

#pragma once

#include <unit_defs.hpp>


namespace bt {

class balance_book {
 private:
    price cash_;
    int n_asset_ = 0;
    price curr_mkt_price = 0;

 public:
    price mkt_price() const {
        return curr_mkt_price;
    }

    balance_book &mkt_price(price px) {
        curr_mkt_price = px;
        return *this;
    }

    price cash() const {
        return cash_;
    }

    int n_asset() const {
        return n_asset_;
    }

    price asset_value() const {
        return n_asset() * mkt_price();
    }

    price mkt_value() const {
        return cash() + asset_value();
    }

    balance_book &sell(size_t amount) {
        auto const vol = -(static_cast<int>(amount) * mkt_price());

        if (static_cast<int>(amount) <= n_asset()
            || abs(vol) <= cash()) {
            take_at(-static_cast<int>(amount), vol);
        }
        return *this;
    }

    balance_book &buy(size_t amount) {
        auto const vol = amount * mkt_price();

        if (vol <= cash()) {
            take_at(amount, vol);
        }
        return *this;
    }

    explicit balance_book(price init_depo)
        : cash_(init_depo) {}

 private:
    void take_at(int amount, price vol) {
        n_asset_ += amount;
        cash_ -= vol;
    }
};
}  // namespace bt
