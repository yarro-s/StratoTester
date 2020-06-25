#pragma once

/// cat templates/backtester.hpp > backtester.hpp
/// cat ../inc/deps.h >> backtester.hpp
/// cat rel_prep.hpp >> backtester.hpp
/// echo "#ifdef BACKTESTER_IMPL" >> backtester.hpp
/// cat rel_prep.cpp >> backtester.hpp 
/// echo "#endif" >> backtester.hpp


/*

*/
#pragma once

#include <vector>
#include <numeric>
#include <iostream>

#include <vector>
#include <iostream>

#include <iostream>
#include <functional>

#include <cmath>
#include <iostream>

#include <cmath>

#include <vector>
#include <set>
#include <string>
#include <unordered_map>
#include <ctime>
#include <sstream>
#include <iostream>

#include <vector>
#include <chrono>
#include <string>

#include <string>

#include <limits>
#include <algorithm>
#include <numeric>
#include <iostream>

#include <cmath>
#include <iostream>














namespace bt
{
    using price = double;
    using weight = double;
    using volume = long long;

    using price_t = std::vector<price>;
    using weight_t = std::vector<weight>;

    using ticker = std::string;

    using time_frame =
        std::chrono::duration<float>;

    namespace tf
    {
        auto constexpr second = time_frame(1.0);
        auto constexpr minute = 60 * second;
        auto constexpr hour = 60 * minute;
        auto constexpr day = 24 * hour;
        auto constexpr week = 7 * day;
        auto constexpr month = 30.42 * day;
        auto constexpr year = 52 * week;
    }
}


namespace bt
{
    using algo_lambda =
        std::function<weight(price_t const &)>;

    class asset_alloc
    {
    private:
        std::function<weight(price_t)> const fa;

    public:
        virtual weight algo(price_t const &price_hist)
        {
            return fa(price_hist);
        }

        virtual weight on_hist(const price_t &price_hist)
        {
            auto const w = algo(price_hist);


            auto const w_hi = 1.0, w_lo = -1.0;

            return w >= w_lo && w <= w_hi
                ? w : (w > w_hi ? w_hi : w_lo);
        }

        asset_alloc()
            : fa([](price_t const &pT) { return 0.0; }) {}

        asset_alloc(algo_lambda fa) : fa(fa) {}

        ~asset_alloc() {}
    };

    class asset_alloc_lb : public asset_alloc
    {
    private:
        size_t n_lback;

    public:
        weight on_hist(const price_t &price_hist)
        {
            auto const len_passed = price_hist.size();
            weight wT = 0.0;

            if (len_passed >= n_lback)
            {
                auto const lb_slice = price_t(
                    price_hist.end()-n_lback,
                    price_hist.end());

                wT = asset_alloc::on_hist(lb_slice);

            }

            return wT;
        }

        asset_alloc_lb(size_t n_lback) : n_lback(n_lback) {}

        asset_alloc_lb(size_t n_lback, algo_lambda fa)
            : asset_alloc::asset_alloc(fa),
              n_lback(n_lback) {}

        ~asset_alloc_lb() {}
    };
}





namespace bt
{
    class backtest
    {

    public:
        virtual backtest &update(price px, weight w) = 0;
        virtual backtest &run(asset_alloc &a_alloc) = 0;
    };
}
namespace bt
{
    class balance_book
    {
    private:
        price cash_;
        int n_asset_ = 0;
        price curr_mkt_price = 0;

    public:
        price mkt_price() const
        {
            return curr_mkt_price;
        }

        balance_book &mkt_price(price px)
        {
            curr_mkt_price = px;
            return *this;
        }

        price cash() const
        {
            return cash_;
        }

        int n_asset() const
        {
            return n_asset_;
        }

        price asset_value() const
        {
            return n_asset() * mkt_price();
        }

        price mkt_value() const
        {
            return cash() + asset_value();
        }

        balance_book &sell(size_t amount)
        {
            auto const vol = -((int)amount * mkt_price());

            if ((int)amount <= n_asset()
                || abs(vol) <= cash())
            {
                take_at(-(int)amount, vol);
            }
            return *this;
        }

        balance_book &buy(size_t amount)
        {
            auto const vol = amount * mkt_price();

            if (vol <= cash())
            {
                take_at(amount, vol);
            }
            return *this;
        }

        balance_book(price init_depo)
            : cash_(init_depo){};

        private:
        void take_at(int amount, price vol)
        {
            n_asset_ += amount;
            cash_ -= vol;
        }
    };
}
namespace bt
{
    class result
    {
    private:
        weight_t wT;

        price_t pvT;

    public:

        void save(balance_book book)
        {
            wT.push_back(book.asset_value() / book.mkt_value());
            pvT.push_back(book.mkt_value());
        }

        weight_t const &wt() const
        {
            return wT;
        }

        price_t const &pv() const
        {
            return pvT;
        }

        price growth() const
        {
            return pv().back() / pv().front();
        }

        price max_drawdown() const;
    };

    class timed_result : public result
    {
    private:
        time_frame const t_frame;

    public:
        price cagr() const
        {
            auto const n_samples = pv().size();
            float const n_years = n_samples * t_frame /
                                  tf::year;
            return pow(growth(), 1.0 / n_years) - 1;
        }

        timed_result(result res, time_frame tf)
            : result(res), t_frame(tf) {}
    };
}








namespace bt
{
    class single_asset : public backtest
    {
    private:
        price_t pT;

        result res;
        balance_book book;

        size_t rb_period = 1;

    public:
        single_asset rebalance_every(size_t period)
        {
            rb_period = period;
            return *this;
        }

        result const &results()
        {
            return res;
        }

        timed_result results(time_frame tf)
        {
            return timed_result(res, tf);
        }

        single_asset &update(price px, weight w);
        single_asset &run(asset_alloc &a_alloc);

        single_asset(const price_t &pT)
            : single_asset(pT, 1.0e18) {}

        single_asset(const price_t &pT,
                     price initial_deposit)
            : pT(pT),
              book(initial_deposit) {}
    };
}
namespace bt
{
    template <typename T>
    class t_series
    {
    protected:
        std::set<std::time_t> t_stamps;

        std::set<ticker> _tickers;
        std::unordered_map<ticker, std::vector<T>> vals;
        std::unordered_map<ticker,
                           std::string> tags;

    public:
        std::vector<T> const &vals_for(ticker const &tckr) const
        {
            return vals.at(tckr);
        }

        std::set<std::time_t> const &timing() const
        {
            return t_stamps;
        }

        std::set<ticker> const &tickers() const
        {
            return _tickers;
        }

        size_t n_tickers() const
        {
            return _tickers.size();
        }

        size_t n_tpoints() const
        {
            return t_stamps.size();
        }

        void append_at(ticker tckr, std::time_t t_stamp,
                       T val)
        {
            _tickers.insert(tckr);

            auto const &pos = t_stamps.insert(t_stamp);

            auto const idxT =
                std::distance(t_stamps.begin(), pos.first);

            vals[tckr].push_back(val);
        }

        ~t_series(){};
    };
}

namespace bt
{
    std::string str_rep(balance_book &book);

    std::string str_rep(t_series<double> const &ts);

    std::string ts_to_str(const price_t &sT);
}
namespace bt
{
    class ma_cross : public asset_alloc_lb
    {
    public:
        weight algo(price_t const &pT)
        {
            auto const sma =
                std::accumulate(pT.begin(), pT.end(), 0)
                    / pT.size();

            auto const p_last = pT.back();
            auto const wT = p_last > sma ? 1.0 : 0.0;

            return wT;
        }

        ma_cross(size_t n_lookback)
            : asset_alloc_lb(n_lookback){};

        ~ma_cross(){};
    };
}





namespace bt
{
    class random_alloc : public asset_alloc
    {
    public:
        weight algo(const price_t &pT)
        {
            return 1.0;
        }

        random_alloc(){};
        ~random_alloc(){};
    };
}
namespace bt
{
    class weight_alloc : public asset_alloc
    {
    private:
        const weight_t &wT;

    public:
        weight algo(const price_t &pT)
        {
            size_t idxT =
                (pT.size()-1) % wT.size();


            return wT[idxT];
        }

        weight_alloc(const weight_t &wT) : wT(wT){};
        ~weight_alloc(){};
    };
}

#ifdef BACKTESTER_IMPL
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
}
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
}
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

}
#endif
