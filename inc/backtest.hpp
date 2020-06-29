#pragma once

#include <unit_defs.hpp>


namespace bt
{
    class backtest
    {

    public:
        virtual backtest &update(price px, weight w) = 0;
        virtual backtest &run(price_t const &pT) = 0;
    };
}

