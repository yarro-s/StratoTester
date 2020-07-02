#pragma once

#ifndef _HEAD_BLOCK
#include <memory>
#endif

#include <asset_alloc.hpp>
#include <asset_alloc_lb.hpp>
#include <asset_alloc_rb.hpp>
#include <strategies/lambda_alloc.hpp>

namespace bt 
{
    template <typename S>
    class strategy : public asset_alloc
    {
    private:
        S strat;

        template <typename U>
        strategy<asset_alloc_lb> _filter_rb(U &strat, size_t n)
        {
            static_assert(!std::is_same<U, asset_alloc_rb>::value,
                          "Specify lookback before rebalancing");
            return strategy<asset_alloc_lb>(strat, n);
        }

    protected:
        weight algo(price_t const &price_hist) override 
        {
            return 0.0;
        }

    public:
        strategy<asset_alloc_rb> rebalance_every(size_t m) 
        {
            return strategy<asset_alloc_rb>(strat, m);
        }

        strategy<asset_alloc_lb> lookback(size_t n) 
        {
            return _filter_rb(strat, n);    // except rb 
        }

        virtual weight on_hist(price_t const &price_hist) override
        {
            return strat.on_hist(price_hist);
        }

        template <class... Args>
        strategy(Args&&... args)
            : strat(args...) {}
    };
}