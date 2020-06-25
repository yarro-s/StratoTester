#pragma once

#include <asset_alloc.hpp>


namespace bt
{
    class random_alloc : public asset_alloc
    {
    public:
        weight algo(const price_t &pT)
        {
            return 1.0; // TO BE IMPLEMENTED
        }

        random_alloc(){};
        ~random_alloc(){};
    };
} // namespace bt
