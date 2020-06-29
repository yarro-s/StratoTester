#pragma once

#ifndef _HEAD_BLOCK
#include <deps.h>
#endif

#include <asset_alloc.hpp>
#include <unit_defs.hpp>


namespace bt
{
    class asset_alloc_rb : public asset_alloc
    {
    private:
        asset_alloc &a_alloc;
        size_t m_rebalance;

    protected:
        weight algo(price_t const &price_hist) override 
        {
            return 0;
        }

    public:
        weight on_hist(price_t const &price_hist) override
        {
            weight wT = 0.0;

            if (!(price_hist.size() % m_rebalance))
            {
                std::cout << std::endl << "  REBALANCING...";
                wT = a_alloc.on_hist(price_hist);
            }
            // std::cout << " wT > : " << wT << std::endl; 
            return wT; 
        }

        asset_alloc_rb(asset_alloc& a_alloc, size_t m) 
            : a_alloc(a_alloc), m_rebalance(m) {}
    };
} // namespace bt
