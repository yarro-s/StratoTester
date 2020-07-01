#pragma once

#ifndef _HEAD_BLOCK
#include <memory>
#endif

#include <asset_alloc.hpp>
#include <asset_alloc_lb.hpp>
#include <asset_alloc_rb.hpp>

namespace bt 
{
    template <typename S>
    class strategy : public asset_alloc
    {
    private:
        S strat;

        strategy<asset_alloc_rb> _filter_lb(
            asset_alloc_lb &with_lb, size_t m)
        {
            return strategy<asset_alloc_rb>(with_lb, m);
        }

    protected:
        weight algo(price_t const &price_hist) override 
        {
            return 0.0;
        }

    public:
        strategy<asset_alloc_rb> rebalance_every(size_t m) 
        {
            return _filter_lb(strat, m);
        }

        strategy<asset_alloc_lb> lookback(size_t n) 
        {
            return strategy<asset_alloc_lb>(strat, n);
        }

        virtual weight on_hist(price_t const &price_hist) override
        {
            return strat.on_hist(price_hist);
        }

        template <class... Args>
        strategy(Args&... args)
            : strat(args...) {}
    };
}