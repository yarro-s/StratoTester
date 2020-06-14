#include <limits>
#include <algorithm>
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
        auto const global_peak = 
            std::max_element(pvT.begin(), pvT.end());
        auto const right_trough =
            std::min_element(global_peak, pvT.end());
        auto const left_trough =
            std::min_element(pvT.begin(), global_peak);

        price max_dd = 0.0;

        if (*left_trough > *right_trough)
        {
            max_dd = -(*global_peak - *right_trough) 
                        / (*global_peak);

            // std::cout << "\nG: " << *left_trough << " / "
            //           << *global_peak << " \\ " << *right_trough
            //           << std::endl;
        }
        else if (*left_trough < *right_trough)
        {
            auto const loc_peak = 
                std::max_element(pvT.begin(), global_peak);
            auto const loc_trough = 
                std::min_element(loc_peak, global_peak);

            max_dd = -(*loc_peak - *loc_trough) 
                        / (*loc_peak);

            // std::cout << "\nL: " << *left_trough << " / "
            //           << *loc_peak << " \\ " << *loc_trough
            //           << std::endl;
        }  
         
        return max_dd;
    }
} // namespace bt