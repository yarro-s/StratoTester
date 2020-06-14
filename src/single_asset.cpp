#include <math.h>
#include <single_asset.hpp>

#include <iostream>
// #include <utils.hpp>

namespace bt
{
    single_asset &single_asset::run(
        asset_alloc &a_alloc)
    {
        res = result(cash_deposit);

        for (auto p = pT.begin(); p != pT.end(); ++p)
        {
            auto const &roll_wnd = price_t(pT.begin(), p+1);
            auto const w = a_alloc.on_hist(roll_wnd);

            res = res.update_pv(*p, w);
        }
        return *this;
    }
} // namespace bt
