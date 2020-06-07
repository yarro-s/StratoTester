#pragma once
#include <unit_defs.hpp>

namespace bt
{
    class asset_alloc
    {
    public:
        virtual weight on_hist(const price_t& price_hist) = 0;
    };
} // namespace bt