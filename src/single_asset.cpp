#include <single_asset.hpp>

#include <iostream>

namespace bt
{
    BTResult single_asset::run(asset_alloc &a_alloc)
    {
        for (auto p = pT.begin(); p != pT.end(); ++p)
        {
            const auto &roll_wnd = price_t(pT.begin(), p);
            const auto wT = a_alloc.on_hist(roll_wnd);
            const double vT = (*p) * wT;

            pvT.push_back(vT);
        }

        return BTResult(pvT);
    }
} // namespace bt
