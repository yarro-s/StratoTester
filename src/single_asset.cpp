#ifndef _SRC_BLOCK
#include <deps.h>

#include <single_asset.hpp>
#endif


namespace bt
{ 
    single_asset &single_asset::update(price px, weight w)
    {
        book.mkt_price(px);

        auto const asset_value_req =
            book.mkt_value() * w;

        auto const n_asset_req =
            trunc(asset_value_req / px);
        
        auto const n_asset_diff = n_asset_req - book.n_asset();
        auto const amount = abs(n_asset_diff);

        book.mkt_price(px);
        // std::cout << std::endl << 
        //     "MKT VAL: " << book.mkt_value() << "   "
        //     "PX:" << px << "   "
        //     "W: " << w << "   "
        //     "VAL REQ: " << asset_value_req << "   "
        //     "N REQ: " << n_asset_req << "   "
        //     "TRADE DEC: " << n_asset_diff << "   "
        //     "N ASSET: " << book.n_asset() << 
        //     std::endl; 

        if (n_asset_diff > 0)
        {
            book.buy(amount);
        }
        else if (n_asset_diff < 0)
        {
            book.sell(amount);
        }
        return *this;
    }
 
    single_asset &single_asset::run(price_t pT)
    {
        weight w = 0.0;
        for (auto p = pT.begin(); p != pT.end(); ++p)
        {
            auto const idxT = std::distance(pT.begin(), p);
            if (idxT % rb_period == 0)
            {   
                // std::cout << std::endl << "w : " << w;
                // std::cout << std::endl << "p : " << *p;
                update(*p, w);
                res.save(book);
            }

            auto const &roll_wnd = price_t(pT.begin(), p+1);
            // std::cout << std::endl
            //           << "rwT: " << *pT.begin() << " -- " << *(p+1);
            w = a_alloc.on_hist(roll_wnd);
        }
        return *this;
    }
} // namespace bt
