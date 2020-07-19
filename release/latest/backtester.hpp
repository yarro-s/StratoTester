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



















namespace bt {
using price = double;
using weight = double;
using volume = long long;

using price_t = std::vector<price>;
using weight_t = std::vector<weight>;

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
namespace bt {
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
namespace bt {

class balance_book {
 private:
    price cash_;
    int n_asset_ = 0;
    price curr_mkt_price = 0;

 public:
    price mkt_price() const {
        return curr_mkt_price;
    }

    balance_book &mkt_price(price px) {
        curr_mkt_price = px;
        return *this;
    }

    price cash() const {
        return cash_;
    }

    int n_asset() const {
        return n_asset_;
    }

    price asset_value() const {
        return n_asset() * mkt_price();
    }

    price mkt_value() const {
        return cash() + asset_value();
    }

    balance_book &sell(size_t amount) {
        auto const vol = -(static_cast<int>(amount) * mkt_price());

        if (static_cast<int>(amount) <= n_asset()
            || abs(vol) <= cash()) {
            take_at(-static_cast<int>(amount), vol);
        }
        return *this;
    }

    balance_book &buy(size_t amount) {
        auto const vol = amount * mkt_price();

        if (vol <= cash()) {
            take_at(amount, vol);
        }
        return *this;
    }

    explicit balance_book(price init_depo)
        : cash_(init_depo) {}

 private:
    void take_at(int amount, price vol) {
        n_asset_ += amount;
        cash_ -= vol;
    }
};
}



namespace bt {
std::string str_rep(balance_book const &book);

std::string str_rep(t_series<double> const &ts);

std::string str_rep(price_t const &sT);
}







namespace bt {
class backtest {
 public:
    virtual backtest &update(price px, weight w) = 0;
    virtual backtest &run(price_t const &pT) = 0;
};
}


namespace bt {

class asset_alloc {
 private:
    backtest *model = nullptr;

 protected:
    virtual weight algo(price_t const &price_hist) = 0;


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

    virtual weight on_hist(price_t const &price_hist) {
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



namespace bt {

class strategy : public asset_alloc {
 private:
    asset_alloc *alloc;

 protected:
    weight algo(price_t const &) override { return 0; }

 public:
    weight on_hist(price_t const &price_hist) override = 0;

    virtual strategy *rebalance_every(size_t m) = 0;

    virtual strategy *set_lookback(size_t n) = 0;

    strategy &set_alloc(asset_alloc *alloc) {
        this->alloc = alloc;
        return *this;
    }

    asset_alloc *get_alloc() {
        return this->alloc;
    }

    explicit strategy(asset_alloc *alloc)
        : alloc(alloc) {}
};
}







namespace bt {

class lookback : public strategy {
 private:
    size_t n_lookback;

 public:
    weight on_hist(price_t const &price_hist) override {
        auto const len_passed = price_hist.size();
        weight wT = 0.0;





        if (len_passed >= n_lookback) {
            auto const t0 = price_hist.end() - n_lookback;
            auto const t_now = price_hist.end();

            wT = get_alloc()->on_hist(price_t(t0, t_now));

            upd_model(price_hist.back(), wT);
        }

        return wT;
    }

    strategy *set_lookback(size_t n) override {
        n_lookback = n;
        return this;
    }

    strategy *rebalance_every(size_t m_rebalance) override;

    lookback(asset_alloc *alloc, size_t n_lookback)
        : strategy(alloc), n_lookback(n_lookback) {}
};
}


namespace bt {

class rebalance : public strategy {
 private:
    size_t m_rebalance;

 public:
    weight on_hist(price_t const &price_hist) override {
        weight wT = 0.0;





        if (!(price_hist.size() % m_rebalance)) {
            wT = get_alloc()->on_hist(price_hist);

            upd_model(price_hist.back(), wT);
        }

        return wT;
    }

    virtual strategy *rebalance_every(size_t m) override {
        m_rebalance = m;
        return this;
    }

    strategy *set_lookback(size_t n) override;

    rebalance(asset_alloc *alloc, size_t m_rebalance)
        : strategy(alloc), m_rebalance(m_rebalance) {}
};
}







namespace bt {

class result {
 private:
    weight_t wT;
    price_t pvT;

 public:
    void save(balance_book const &book) {
        wT.push_back(book.asset_value() / book.mkt_value());
        pvT.push_back(book.mkt_value());
    }

    weight_t const &wt() const {
        return wT;
    }

    price_t const &pv() const {
        return pvT;
    }

    price growth() const {
        return pv().back() / pv().front();
    }

    price max_drawdown() const;
};

class timed_result : public result {
 private:
    time_frame const t_frame;

 public:
    price cagr() const {
        auto const n_samples = pv().size();
        float const n_years = n_samples * t_frame /
                                tf::year;
        return pow(growth(), 1.0 / n_years) - 1;
    }

    timed_result(result res, time_frame tf)
        : result(res), t_frame(tf) {}
};
}


namespace bt {

class single_asset : public backtest {
 private:
    result res;
    balance_book book;
    asset_alloc &a_alloc;

 public:
    result const &results() {
        return res;
    }

    timed_result results(time_frame tf) {
        return timed_result(res, tf);
    }

    single_asset &update(price px, weight w);
    single_asset &run(price_t const &pT);

    explicit single_asset(asset_alloc &a_alloc)
        : single_asset(a_alloc, 1.0e18) {}

    single_asset(asset_alloc &a_alloc, price initial_deposit)
        : book(initial_deposit), a_alloc(a_alloc) {
        a_alloc.set_model(this);
    }
};
}








namespace bt {

class const_alloc : public asset_alloc {
 private:
    weight const weight_;

 protected:
    weight algo(price_t const &) noexcept {
        return weight_;
    }

 public:
    explicit const_alloc(weight w) noexcept
        : weight_(w) {}

    ~const_alloc() {}
};
}


namespace bt {

class buy_and_hold : public strategy {
 private:
    bool has_updated = false;

 public:
    weight on_hist(price_t const &price_hist) override {
        auto const wT = get_alloc()->on_hist(price_hist);

        if (!has_updated) {
            upd_model(price_hist.back(), wT);

            has_updated = true;
        }
        return wT;
    }

    strategy *rebalance_every(size_t) override {
        return this;
    }

    strategy *set_lookback(size_t) override {
        return this;
    }

    buy_and_hold(weight const w)
        : strategy(new const_alloc(w)) {}
};
}




namespace bt {

class weight_alloc : public asset_alloc {
 private:
    weight_t const &weights;

 public:
    weight algo(price_t const &prices) noexcept {
        size_t idx_weight = (prices.size() - 1) % weights.size();




        return weights[idx_weight];
    }

    explicit weight_alloc(weight_t const &weights) noexcept
        : weights(weights) {}

    ~weight_alloc() {}
};
}

namespace bt {
using algo_lambda =
    std::function<weight(price_t const &)>;

class lambda_alloc : public asset_alloc {
 private:
    std::function<weight(price_t)> const fa;

 public:
    virtual weight algo(price_t const &price_hist) {
        return fa(price_hist);
    }

    lambda_alloc()
        : fa([](price_t const &) {

            return 0.0; }) {}

    explicit lambda_alloc(algo_lambda fa) : fa(fa) {}
};
}

