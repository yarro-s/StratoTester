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
} // namespace bt
