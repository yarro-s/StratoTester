#pragma once

#include <vector>
#include <set>
#include <string>
#include <unordered_map>
#include <ctime>
#include <sstream>
#include <unit_defs.hpp>

#include <iostream>

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
} // namespace bt
