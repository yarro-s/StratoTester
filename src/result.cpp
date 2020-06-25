#ifndef _SRC_BLOCK
#include <deps.h>

#include <result.hpp>
#endif


namespace bt
{
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

