#pragma once
#include <result.hpp>
#include <asset_alloc.hpp>

namespace bt
{
    class backtest
    {
    public:
        virtual backtest &run(asset_alloc &a_alloc) = 0;
    };
} // namespace bt
