#pragma once

#include <cmath>
#include <iostream>
#include <unit_defs.hpp>

namespace bt
{
    class balance_book
    {
    private:
        price cash_;
        int n_asset_ = 0;

    public:
        price cash() const
        {
            return cash_;
        }

        int n_asset() const
        {
            return n_asset_;
        }

        price mkt_value_at(price px) const
        {
            return cash_ + n_asset_ * px;
        }

        balance_book &sell_at(size_t amount, price px)
        {
            auto const vol = -(amount * px);
            
            if ((int)amount <= n_asset() 
                || abs(vol) <= cash())
            {
                take_at(-amount, vol);
            }
            return *this;
        }

        balance_book &buy_at(size_t amount, price px)
        {
            auto const vol = amount * px;

            if (vol <= cash())
            {
                take_at(amount, vol);
            }
            return *this;
        }

        balance_book(price init_depo)
            : cash_(init_depo){};

        private:
        void take_at(int amount, price vol)
        {
            n_asset_ += amount;
            cash_ -= vol;
        }
    };
} // namespace bt
