#pragma once
#include <asset_alloc.hpp>

namespace bt
{
    class random_alloc : public asset_alloc
    {
    public:
        weight on_hist(const price_t &price_hist)
        {
            return 1.0; // TO BE IMPLEMENTED
        }

        random_alloc(){};
        ~random_alloc(){};
    };
} // namespace bt
