#pragma once

#include <iostream>
#include <functional>
#include <unit_defs.hpp>

// #include <utils.hpp>

namespace bt
{
    using algo_lambda = 
        std::function<weight(price_t const &)>;

    class asset_alloc
    {
    private:
        std::function<weight(price_t)> const fa;

    public:
        virtual weight algo(price_t const &price_hist)
        {
            return fa(price_hist);
        }

        virtual weight on_hist(const price_t &price_hist)
        {
            auto const w = algo(price_hist);

            // clamp to 0...1
            auto const w_hi = 1.0, w_lo = -1.0;

            return w >= w_lo && w <= w_hi 
                ? w : (w > w_hi ? w_hi : w_lo);
        }

        asset_alloc()
            : fa([](price_t const &pT) { return 0.0; }) {}

        asset_alloc(algo_lambda fa) : fa(fa) {}

        ~asset_alloc() {}
    };

    class asset_alloc_lb : public asset_alloc
    {
    private:
        size_t n_lback;

    public:
        weight on_hist(const price_t &price_hist)
        {
            auto const len_passed = price_hist.size();
            weight wT = 0.0;

            if (len_passed >= n_lback)
            {
                auto const lb_slice = price_t(
                    price_hist.end()-n_lback, 
                    price_hist.end());

                wT = asset_alloc::on_hist(lb_slice);
                // return wT;
            }
            // std::cout << " wT > : " << wT << std::endl; 
            return wT; 
        }

        asset_alloc_lb(size_t n_lback) : n_lback(n_lback) {}

        asset_alloc_lb(size_t n_lback, algo_lambda fa)
            : asset_alloc::asset_alloc(fa),
              n_lback(n_lback) {}

        ~asset_alloc_lb() {}
    };
} // namespace bt