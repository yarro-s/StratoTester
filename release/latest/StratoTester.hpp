/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <iostream>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

#include <cmath>
#include <chrono>

#include <functional>
#include <iomanip>
namespace st {
using price = double;
using weight = double;
using volume = long long;

using prices = std::vector<price>;
using weights = std::vector<weight>;

using ticker = std::string;

using time_frame = std::chrono::duration<float>;

namespace tf {
    auto constexpr second = time_frame(1.0);
    auto constexpr minute = 60 * second;
    auto constexpr hour = 60 * minute;
    auto constexpr day = 24 * hour;
    auto constexpr week = 7 * day;
    auto constexpr month = 30.42 * day;
    auto constexpr year = 52 * week;
}
}


namespace st {
class backtest {
 public:
    virtual backtest &update(price px, weight w) = 0;
    virtual backtest &run(prices const &pT) = 0;
};
}
namespace st {
template <typename T>
class t_series {
 protected:
    std::set<std::time_t> t_stamps;

    std::set<ticker> _tickers;
    std::unordered_map<ticker, std::vector<T>> vals;
    std::unordered_map<ticker, std::string> tags;

 public:
    std::vector<T> const &vals_for(ticker const &tckr) const {
        return vals.at(tckr);
    }

    std::set<std::time_t> const &timing() const noexcept {
        return t_stamps;
    }

    std::set<ticker> const &tickers() const noexcept {
        return _tickers;
    }

    size_t n_tickers() const noexcept {
        return _tickers.size();
    }

    size_t n_tpoints() const noexcept {
        return t_stamps.size();
    }

    void append_at(ticker tckr, std::time_t t_stamp, T val) noexcept {
        _tickers.insert(tckr);

        t_stamps.insert(t_stamp);

        vals[tckr].push_back(val);
    }
};
}
namespace st {

class balance_book {
 private:
    price cash_;
    int n_asset_;
    price curr_mkt_price;

    price _init_depo;

 public:
    void reset() {
        cash_ = _init_depo;
        n_asset_ = 0;
        curr_mkt_price = 0;
    }

    price mkt_price() const { return curr_mkt_price; }

    balance_book &mkt_price(price px) {
        curr_mkt_price = px;
        return *this;
    }

    price cash() const noexcept { return cash_; }

    int n_asset() const noexcept { return n_asset_; }

    price asset_value() const noexcept { return n_asset() * mkt_price(); }

    price mkt_value() const noexcept { return cash() + asset_value(); }

    balance_book &sell(size_t amount) noexcept {
        auto const vol = -(static_cast<int>(amount) * mkt_price());

        if (static_cast<int>(amount) <= n_asset()
            || abs(vol) <= cash()) {
            take_at(-static_cast<int>(amount), vol);
        }
        return *this;
    }

    balance_book &buy(size_t amount) noexcept {
        auto const vol = amount * mkt_price();

        if (vol <= cash()) {
            take_at(amount, vol);
        }
        return *this;
    }

    explicit balance_book(price init_depo) noexcept
        : _init_depo(init_depo) {
        reset();
    }

 private:
    void take_at(int amount, price vol) noexcept {
        n_asset_ += amount;
        cash_ -= vol;
    }
};
}



namespace st {

std::time_t str_to_time(std::string const &time_str,
                        std::string const &time_fmt = "%D");

std::string str_rep(time_t const &t_stamp,
                    std::string const &time_fmt = "%D");

std::string str_rep(balance_book const &book);

std::string str_rep(t_series<double> const &ts);

std::string str_rep(prices const &sT);
}



namespace st {

class asset_alloc {
 private:
    backtest *model = nullptr;

 protected:
    virtual weight algo(prices const &price_hist) = 0;


    backtest &upd_model(price px, weight w) {
        if (model)
            return this->model->update(px, w);
        else
            return *this->model;
    }

 public:
    asset_alloc &set_model(backtest *model) {
        this->model = model;
        return *this;
    }

    virtual weight on_hist(prices const &price_hist) {
        auto w = algo(price_hist);


        auto const w_hi = 1.0, w_lo = -1.0;
        w = w >= w_lo && w <= w_hi ? w
                                      : (w > w_hi ? w_hi : w_lo);
        upd_model(price_hist.back(), w);
        return w;
    }

    virtual ~asset_alloc() {}
};
}

namespace st {

class result {
 private:
    weights wT;
    prices pvT;

 public:
    void reset() noexcept {
        wT.clear();
        pvT.clear();
    }

    void save(balance_book const &book) noexcept {
        wT.push_back(book.asset_value() / book.mkt_value());
        pvT.push_back(book.mkt_value());
    }

    weights const &asset_weights() const noexcept { return wT; }

    prices const &value_history() const noexcept { return pvT; }

    price total_return() const noexcept { return value_history().back() / value_history().front(); }

    price max_drawdown() const noexcept;
};

class timed_result : public result {
 private:
    time_frame const t_frame;

 public:
    price cagr() const noexcept {
        auto const n_samples = value_history().size();
        float const n_years = n_samples * t_frame / tf::year;
        return pow(total_return(), 1.0 / n_years) - 1;
    }

    timed_result(result res, time_frame tf) noexcept
        : result(res), t_frame(tf) {}
};
}


namespace st {

class single_asset : public backtest {
 private:
    result res;
    balance_book book;
    asset_alloc &a_alloc;

    weight _last_w;

    void reset() noexcept {
        book.reset();
        res.reset();
        _last_w = 0.0;
    }

 public:
    result results() noexcept { return res; }

    timed_result results(time_frame tf) noexcept {
        return timed_result(res, tf);
    }

    single_asset &update(price px, weight w);
    single_asset &run(prices const &pT);

    explicit single_asset(asset_alloc &a_alloc)
        : single_asset(a_alloc, 1.0e12) {}

    single_asset(asset_alloc &a_alloc, price initial_deposit)
        : book(initial_deposit), a_alloc(a_alloc) {
        reset();
        a_alloc.set_model(this);
    }
};
}
namespace st {

class chained_alloc : public asset_alloc {
 private:
    asset_alloc *next_alloc;

 protected:
    weight algo(prices const &price_hist) override {
        return next_alloc->on_hist(price_hist);
    }

 public:
    asset_alloc &set_next(asset_alloc *next_alloc) {
        this->next_alloc = next_alloc;
        return *this;
    }

    asset_alloc *get_next() { return this->next_alloc; }

    weight on_hist(prices const &price_hist) override {
        weight wT = algo(price_hist);
        upd_model(price_hist.back(), wT);

        return wT;
    }

    explicit chained_alloc(asset_alloc *alloc)
        : next_alloc(alloc) {}
};
}
namespace st {

class with_lookback : public chained_alloc {
 private:
    size_t m_lookback;

 protected:
    weight algo(prices const &price_hist) override {
        auto const t0 = price_hist.end() - m_lookback;
        auto const t_now = price_hist.end();

        return chained_alloc::algo(prices(t0, t_now));
    }

 public:
    weight on_hist(prices const &price_hist) override {
        auto const len_passed = price_hist.size();
        weight wT = 0.0;

        if (len_passed >= m_lookback) {
            wT = algo(price_hist);
            upd_model(price_hist.back(), wT);
        }

        return wT;
    }

    with_lookback(asset_alloc *next_alloc, size_t m_lookback)
        : chained_alloc(next_alloc), m_lookback(m_lookback) {}
};
}
namespace st {

class with_rebalance : public chained_alloc {
 private:
    size_t n_rebalance;

 public:
    weight on_hist(prices const &price_hist) override {
        weight wT = algo(price_hist);

        if (!(price_hist.size() % n_rebalance)) {
            upd_model(price_hist.back(), wT);

            return wT;
        } else {
            return 0.0;
        }
    }

    with_rebalance(asset_alloc *next_alloc, size_t n_rebalance)
        : chained_alloc(next_alloc), n_rebalance(n_rebalance) {}
};
}


namespace st {

class strategy : public chained_alloc {
 private:
    bool rebalancing_set = false;

 public:
    virtual strategy &look_back(size_t m) {
        if (rebalancing_set) {
            auto my_rebalanced_alloc =
                static_cast<chained_alloc*>(get_next());
            auto lookback_alloc =
                new with_lookback(my_rebalanced_alloc->get_next(), m);
            my_rebalanced_alloc->set_next(lookback_alloc);
        } else {
            set_next(new with_lookback(get_next(), m));
        }
        return *this;
    }

    virtual strategy &rebalance_every(size_t n) {
        set_next(new with_rebalance(get_next(), n));
        rebalancing_set = true;
        return *this;
    }

    explicit strategy(asset_alloc *alloc)
        : chained_alloc(alloc) {}
};
}





namespace st {

class weight_alloc : public asset_alloc {
 private:
    weights const &weight_hist;

 public:
    weight algo(prices const &price_hist) noexcept {
        size_t idx_weight = (price_hist.size() - 1) % weight_hist.size();




        return weight_hist[idx_weight];
    }

    explicit weight_alloc(weights const &weight_hist) noexcept
        : weight_hist(weight_hist) {}

    ~weight_alloc() {}
};
}
namespace st {

class const_alloc : public asset_alloc {
 private:
    bool has_updated = false;

    weight const weight_;

 protected:
    weight algo(prices const &) override { return weight_; }

    weight on_hist(prices const &price_hist) override {
        auto const wT = algo(price_hist);

        if (!has_updated) {
            upd_model(0, wT);
            upd_model(price_hist.back(), wT);

            has_updated = true;
        }




        return wT;
    }

 public:
    explicit const_alloc(weight w) noexcept
        : weight_(w) {}

    ~const_alloc() {}
};
}



namespace st {
using algo_lambda =
    std::function<weight(prices const &)>;

class lambda_alloc : public asset_alloc {
 private:
    std::function<weight(prices)> const fa;

 public:
    virtual weight algo(prices const &price_hist) {
        return fa(price_hist);
    }

    explicit lambda_alloc(algo_lambda const &fa) : fa(fa) {}
};
}

#ifdef STRATOTESTER_IMPL
namespace st {
price result::max_drawdown() const noexcept {
    price max_dd = 0.0,
          gmax = 0.0,
          gmin = 0.0;

    for (auto pv0 = pvT.begin(); pv0 != pvT.end() - 1; ++pv0) {
        for (auto pv1 = pv0 + 1; pv1 != pvT.end(); ++pv1) {
            auto const loc_dd = *pv0 - *pv1;

            if (loc_dd > max_dd) {
                max_dd = loc_dd;
                gmax = *pv0;
                gmin = *pv1;
            }
        }
    }

    return gmax == gmin ? 0.0 : -(gmax - gmin)/gmax;
}
}
namespace st {

single_asset &single_asset::update(price px, weight w) {
    px = book.mkt_price();
    auto const asset_value_req = book.mkt_value() * _last_w;
    auto const n_asset_req = trunc(asset_value_req / px);

    auto const n_asset_diff = n_asset_req - book.n_asset();
    auto const amount = abs(n_asset_diff);

    if (n_asset_diff > 0) {
        book.buy(amount);
    } else if (n_asset_diff < 0) {
        book.sell(amount);
    }

    _last_w = w;
    return *this;
}

single_asset &single_asset::run(prices const &pT) {
    reset();

    for (auto p = pT.begin(); p != pT.end(); ++p) {
        auto const &roll_wnd = prices(pT.begin(), p + 1);

        book.mkt_price(*p);
        a_alloc.on_hist(roll_wnd);
        res.save(book);
    }
    return *this;
}
}
namespace st {

std::time_t str_to_time(std::string const &time_str,
                        std::string const &time_fmt) {
    std::tm tm = std::tm();

    std::istringstream ss(time_str);
    ss >> std::get_time(&tm, time_fmt.c_str());

    return mktime(&tm);
}

std::string str_rep(time_t const &t_stamp, std::string const &time_fmt) {
    std::tm tm = std::tm();
    localtime_r(&t_stamp, &tm);

    char buff[20]{0};

    strftime(buff, 20, time_fmt.c_str(), &tm);
    return std::string(buff);
}

std::string str_rep(balance_book const &book) {
    std::stringstream rep;

    rep << book.cash() << " & " << book.n_asset();

    return rep.str();
}

std::string str_rep(t_series<double> const &ts) {
    std::stringstream rep;

    rep << "         ";

    for (auto const &tckr : ts.tickers()) {
        rep << tckr << "   ";
    }
    rep << std::endl;

    for (auto t_stamp = ts.timing().begin();
            t_stamp != ts.timing().end(); ++t_stamp) {
        auto const idXT = std::distance(
            ts.timing().begin(), t_stamp);

        rep << str_rep(*t_stamp);

        for (auto const &tckr : ts.tickers()) {
            rep << " " << ts.vals_for(tckr).at(idXT);
        }

        rep << std::endl;
    }
    return rep.str();
}

std::string str_rep(prices const &sT) {
    std::ostringstream ss;
    ss << '[';
    bool first = true;
    for (price const &elem : sT) {
        if (!first) {
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
