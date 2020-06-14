#pragma once
#include <unit_defs.hpp>
#include <backtest.hpp>

namespace bt
{
    class single_asset : public backtest
    {
    private:
        price_t pT;
        price const cash_deposit;

        result res;

    public:
        result const &results()
        {
            return res;
        }

        timed_result results(time_frame tf)
        {
            return timed_result(res, tf);
        }

        single_asset &run(asset_alloc &a_alloc);

        single_asset(const price_t &pT)
            : single_asset(pT, 1.0e18) {}

        single_asset(const price_t &pT,
                     price initial_deposit)
            : pT(pT),
              cash_deposit(initial_deposit) {}

        ~single_asset() {}
    };
} // namespace bt