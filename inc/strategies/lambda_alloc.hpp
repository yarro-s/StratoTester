#pragma once

#ifndef _HEAD_BLOCK
#include <deps.h>
#endif

#include <asset_alloc.hpp>


namespace bt
{
    using algo_lambda = 
        std::function<weight(price_t const &)>;

    class lambda_alloc : asset_alloc
    {
    private:
        std::function<weight(price_t)> const fa;

    public:
        virtual weight algo(price_t const &price_hist)
        {
            return fa(price_hist);
        }

        lambda_alloc()
            : fa([](price_t const &pT) { 
                std::cout << "DEFAULT ALGO LAMBDA!" << std::endl;
                return 0.0; }) {}

        lambda_alloc(algo_lambda fa) : fa(fa) {}
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
