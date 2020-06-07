#pragma once
#include <BTResult.hpp>
#include <asset_alloc.hpp>

namespace bt
{
    class backtest
    {
    public:
        virtual BTResult run(asset_alloc& a_alloc) = 0;
    };
} // namespace bt
