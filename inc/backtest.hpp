#pragma once
#include <result.hpp>
#include <asset_alloc.hpp>

namespace bt
{
    class backtest
    {
    public:
        virtual result const &results() = 0;
        virtual timed_result results(time_frame tf) = 0;
        virtual backtest &run(asset_alloc &a_alloc) = 0;
    };
} // namespace bt
