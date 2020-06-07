#pragma once
#include <unit_defs.hpp>
#include <backtest.hpp>

namespace bt
{
    class single_asset : public backtest
    {
    private:
        const price_t &pT;

        price_t pvT;

    public:
        BTResult run(asset_alloc& a_alloc);

        single_asset(const price_t &pT) : pT(pT) {}
        ~single_asset() {}
    };
} // namespace bt