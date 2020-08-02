/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _SRC_BLOCK
#include <iostream>  // debug

#include <single_asset.hpp>
#include <utils.hpp>
#endif


namespace bt {

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
/*
    std::cout << std::endl << "   "
              << "P(T) = " << px << "   "
              << "W(T) = " << w << "   "
              << "AV req = " << asset_value_req << "   "
              << "N asset req = " << n_asset_req << "   "
              << "delta N req = " << n_asset_diff << "   "
              << "PV(T) = " << book.mkt_value() << "   " << std::endl;
*/
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
}  // namespace bt
