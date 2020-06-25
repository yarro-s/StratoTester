#define _SRC_BLOCK

/// cat templates/rel.cpp > rel.cpp
///     cat ../src/result.cpp >> rel.cpp ....
/// cat ../src/*.cpp >> rel.cpp
/// g++ -P -E rel.cpp -o rel_prep.cpp

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
        std::cout << std::endl << 
            "MKT VAL: " << book.mkt_value() << "   "
            "PX:" << px << "   "
            "W: " << w << "   "
            "VAL REQ: " << asset_value_req << "   "
            "N REQ: " << n_asset_req << "   "
            "TRADE DEC: " << n_asset_diff << "   "
            "N ASSET: " << book.n_asset() << 
            std::endl; 

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
 
    single_asset &single_asset::run(asset_alloc &a_alloc)
    {
        weight w = 0.0;
        for (auto p = pT.begin(); p != pT.end(); ++p)
        {
            auto const idxT = std::distance(pT.begin(), p);
            if (idxT % rb_period == 0)
            {   
                update(*p, w);
                res.save(book);
            }

            auto const &roll_wnd = price_t(pT.begin(), p+1);
            w = a_alloc.on_hist(roll_wnd);
        }
        return *this;
    }
} // namespace bt
#ifndef _SRC_BLOCK
#include <t_series.hpp>
#endif

#ifndef _SRC_BLOCK
#include <utils.hpp>
#endif


namespace bt
{
    std::string str_rep(balance_book &book)
    {
        std::stringstream rep;

        rep << book.cash() << " & " << book.n_asset();

        return rep.str();
    }

    std::string str_rep(t_series<double> const &ts)
    {
        std::stringstream rep;

        auto const t_stamp_to_str =
        [](std::time_t const &t_stamp) {
            struct tm *timeinfo;
            size_t const n_t = 9;
            char buffer[9];

            timeinfo = localtime(&t_stamp);
            strftime(buffer, n_t, "%D", timeinfo);

            return std::string(buffer);
        };

        rep << "         ";

        for (auto const &tckr : ts.tickers())
        {
            rep << tckr << "   ";
        }
        rep << std::endl;

        for (auto t_stamp = ts.timing().begin();
             t_stamp != ts.timing().end(); ++t_stamp)
        {
            auto const idXT = std::distance(
                ts.timing().begin(), t_stamp);

            rep << t_stamp_to_str(*t_stamp);

            for (auto const &tckr : ts.tickers())
            {
                rep << " " << ts.vals_for(tckr).at(idXT);
            }

            rep << std::endl;
        }
        return rep.str();
    }

    std::string ts_to_str(const price_t &sT)
    {
        std::ostringstream ss;
        ss << '[';
        bool first = true;
        for (price const &elem : sT)
        {
            if (!first)
            {
                ss << ", ";
            }
            ss << elem;
            first = false;
        }
        ss << ']';
        return ss.str();
    }

} // namespace bt

