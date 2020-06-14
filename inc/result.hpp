#pragma once

#include <math.h>
#include <unit_defs.hpp>

namespace bt
{   
    class result
    {
    private:
        price cash;
        volume vol = 0;
        price assets = 0;

        weight_t wT;

        price p_prev = 0.0;
        weight w_prev = 0.0;

    protected:
        price_t pvT;

    public:
        weight_t const &wt() const
        {
            return wT;
        }

        price_t const &pv() const
        {
            return pvT;
        }

        result &update_pv(price p, weight w);

        price growth() const
        {
            return pvT.back() / pvT.front();
        }

        price max_drawdown() const;
        
        result(price cash_deposit) : cash(cash_deposit) {}
        result() : result(0.0) {}
        result(result const &res) : pvT(res.pvT) {}

        ~result() {}
    };

    class timed_result : public result
    {
    private:
        time_frame const t_frame;

    public:
        price cagr() const
        {
            auto const n_samples = pvT.size();
            float const n_years = n_samples * t_frame /
                                  tf::year;
            return pow(growth(), 1.0 / n_years) - 1;
        }

        timed_result(result res, time_frame tf)
            : result(res), t_frame(tf) {}

        ~timed_result() {}
    };
} // namespace bt
