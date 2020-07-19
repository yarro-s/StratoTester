/*
 * Copyright (c) Yaroslav Shkurat.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef _SRC_BLOCK
#include <single_asset.hpp>
#include <utils.hpp>
#endif

namespace bt {

single_asset &single_asset::update(price px, weight w) {
    // std::cout << std::endl
    //           << " SA: GETTING UPDATED: "
    //           << px << ", " << w << std::endl;

    px = book.mkt_price();
    auto const asset_value_req = book.mkt_value() * w;
    auto const n_asset_req = trunc(asset_value_req / px);

    auto const n_asset_diff = n_asset_req - book.n_asset();
    auto const amount = abs(n_asset_diff);

    if (n_asset_diff > 0) {
        book.buy(amount);
    } else if (n_asset_diff < 0) {
        book.sell(amount);
    }

    return *this;
}

single_asset &single_asset::run(price_t const &pT) {
    // weight w = 0.0;

    for (auto p = pT.begin()+1; p != pT.end(); ++p) {
        auto const &roll_wnd = price_t(pT.begin(), p);

        // std::cout << std::endl
        //           << "   SA:"
        //           << " P = " << *p << std::endl
        //           << " H  = " << str_rep(roll_wnd) << std::endl;

        book.mkt_price(*p);
        a_alloc.on_hist(roll_wnd);
        res.save(book);
    }
    return *this;
}
}  // namespace bt
