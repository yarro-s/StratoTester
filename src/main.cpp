#include <iostream>
#include <vector>
#include <unistd.h>
#include <BTLight.hpp>

#include <csv.h>

namespace bt
{
    typedef struct
    {
        long idx;
        price price;
        int dir;
        unsigned vol;
        bt::price cash_left;
    } position;

    std::string str_rep(position pos)
    {
        std::ostringstream ss;

        ss << "@[ " << pos.idx << " ]" << "   "
           << "P = " << pos.price << "   "
           << "V = " << pos.dir * 
                static_cast<long>(pos.vol) << "   ";

        return ss.str();
    }
} // namespace bt

int main()
{
    bt::price_t pT  = 
        {10.0, 15.0, 12.0, 20.0, 22.0, 18.0, 35.0};
    std::vector<int> trdT = 
        {   0,    1,    0,    0,   -1,    0,    0};
    std::vector<unsigned> volT = 
        {   0,   10,    0,    0,   10,    0,    0};
    bt::price cash = 200.0;

    auto const tr_exp = 270.0 / 200.0;

    std::cout << "EXP TR: "                   // = 1.46667
              << tr_exp << std::endl;

    // algo
    std::vector<bt::position> posT;

    for (auto p = pT.begin(); p != pT.end(); ++p)
    {
        auto const idx = std::distance(pT.begin(), p);
        auto const trd = trdT[idx];

        if (trd != 0)
        {
            auto const vol = volT[idx];
            posT.push_back({idx, *p, trd, vol});
        }
    }

    for (auto const &pos : posT)
    {
        std::cout << " >>> " << bt::str_rep(pos)
                  << std::endl;
    }

    return 0;
}