#pragma once

#include <string>
#include <t_series.hpp>
#include <balance_book.hpp>
#include <unit_defs.hpp>

namespace bt
{
    std::string str_rep(balance_book &book);

    std::string str_rep(t_series<double> const &ts);

    std::string ts_to_str(const price_t &sT);
} // namespace bt