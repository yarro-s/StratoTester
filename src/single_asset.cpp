// Copyright 2020 Yarro S

#ifndef _SRC_BLOCK
// #include <deps.h>

#include <single_asset.hpp>
#endif

namespace bt {

single_asset &single_asset::update(price px, weight w) {
    book.mkt_price(px);

    auto const asset_value_req = book.mkt_value() * w;
    auto const n_asset_req = trunc(asset_value_req / px);

    auto const n_asset_diff = n_asset_req - book.n_asset();
    auto const amount = abs(n_asset_diff);

    /*
    std::cout << std::endl <<
        "MKT VAL: " << book.mkt_value() << "   "
        "PX:" << px << "   "
        "W: " << w << "   "
        "VAL REQ: " << asset_value_req << "   "
        "N REQ: " << n_asset_req << "   "
        "TRADE DEC: " << n_asset_diff << "   "
        "N ASSET: " << book.n_asset() <<
        std::endl; */

    if (n_asset_diff > 0) {
        book.buy(amount);
    } else if (n_asset_diff < 0) {
        book.sell(amount);
    }

    res.save(book);
    return *this;
}

single_asset &single_asset::run(price_t const &pT) {
    weight w = 0.0;

    for (auto p = pT.begin(); p != pT.end(); ++p) {
        update(*p, w);

        auto const &roll_wnd = price_t(pT.begin(), p+1);
        w = a_alloc.on_hist(roll_wnd);
    }
    return *this;
}
}  // namespace bt
