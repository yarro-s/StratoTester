#pragma once

#ifndef _HEAD_BLOCK
#include <memory>
#endif

#include <asset_alloc.hpp>
#include <asset_alloc_lb.hpp>
#include <asset_alloc_rb.hpp>

namespace bt 
{
    class strategy : public asset_alloc
    {
    private:
        asset_alloc *strat;
    
    protected:
        weight algo(price_t const &price_hist) override 
        {
            return 0.0;
        }

    public:
        strategy &rebalance_every(size_t m) 
        {
            strat = new strategy(new asset_alloc_rb(*strat, m));
            return *this;
        }

        strategy &lookback(size_t n) 
        {
            strat = new strategy(new asset_alloc_lb(*strat, n));
            return *this;
        }

        virtual weight on_hist(price_t const &price_hist) override
        {
            return strat->on_hist(price_hist);
        }

        strategy(asset_alloc *a_alloc)
            : strat(a_alloc) {}
    };
}