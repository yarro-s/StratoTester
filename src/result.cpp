#include <limits>
#include <algorithm>
#include <numeric>
#include <result.hpp>

#include <iostream>

namespace bt
{
    result &result::update_pv(price p, weight w)
    {
        price const delta_p = p - p_prev;
        weight const delta_w = w - w_prev;
        volume const delta_vol =
            trunc(delta_w * cash / p);

        price const delta_assets = p * delta_vol +
                                   vol * delta_p;
        price const delta_cash = -delta_vol * p;

        vol += delta_vol;
        assets += delta_assets;
        cash += delta_cash;

        price const npvT = assets + cash;
        p_prev = p, w_prev = w;

        /*
        std::cout << "RUN: W = " << w << " | "
                  << "dP = " << delta_p << " | "
                  << "dW = " << delta_w << " | "
                  << "dV = " << delta_vol << " | "
                  << "dA = " << delta_assets << " | "
                  << "dC = " << delta_cash << " | "
                  << "V = " << vol << " | "
                  << "A = " << assets << " | "
                  << "C = " << cash << " | "
                  << "N = " << npvT << std::endl;
        */
        
        wT.push_back(w);
        pvT.push_back(npvT);
        
        return *this;
    }

    price result::max_drawdown() const
    {
        price max_dd = 0.0,
              gmax = 0.0,
              gmin = 0.0;

        for (auto pv0 = pvT.begin();
             pv0 != pvT.end() - 1; ++pv0)
        {
            for (auto pv1 = pv0 + 1;
                 pv1 != pvT.end(); ++pv1)
            {
                auto const loc_dd = *pv0 - *pv1;

                if (loc_dd > max_dd)
                {
                    max_dd = loc_dd;
                    gmax = *pv0;
                    gmin = *pv1;
                }
            }
        }
        
        return gmax == gmin ? 0.0 : -(gmax - gmin)/gmax;
    }
} // namespace bt