#pragma once

#ifndef _HEAD_BLOCK
#include <deps.h>
#endif

#include <asset_alloc.hpp>
#include <unit_defs.hpp>


namespace bt
{
    class asset_alloc_lb : public asset_alloc
    {
    private:
        asset_alloc &a_alloc;
        size_t n_lback;

    protected:
        weight algo(price_t const &price_hist) override 
        {
            return 0;
        }

    public:
        weight on_hist(price_t const &price_hist) override
        {
            auto const len_passed = price_hist.size();
            weight wT = 0.0;

            if (len_passed >= n_lback)
            {
                auto const lb_slice = price_t(
                    price_hist.end()-n_lback, 
                    price_hist.end());
                std::cout << std::endl << "  LOOKING BACK...";
                wT = a_alloc.on_hist(lb_slice);
            }
            // std::cout << " wT > : " << wT << std::endl; 
            return wT; 
        }

        asset_alloc_lb(asset_alloc& a_alloc, size_t n) 
            : a_alloc(a_alloc), n_lback(n) {}
    };
} // namespace bt
