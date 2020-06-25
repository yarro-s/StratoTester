#pragma once

#ifndef _HEAD_BLOCK
#include <deps.h>
#endif

#include <asset_alloc.hpp>
#include <utils.hpp>


namespace bt
{
    class ma_cross : public asset_alloc_lb
    {
    public:
        weight algo(price_t const &pT)
        {
            auto const sma = 
                std::accumulate(pT.begin(), pT.end(), 0) 
                    / pT.size();

            auto const p_last = pT.back();
            auto const wT = p_last > sma ? 1.0 : 0.0;
            
            return wT;
        }

        ma_cross(size_t n_lookback)
            : asset_alloc_lb(n_lookback){};
            
        ~ma_cross(){};
    };
} // namespace bt
