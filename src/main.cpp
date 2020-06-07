#include <iostream>
#include <BTLight.hpp>

using namespace bt;

void print_tseries(const price_t &sT)
{
    std::cout << std::endl;
    for (const auto &s : sT)
        std::cout << " " << s;
    std::cout << std::endl;
}

int main()
{
    std::cout << "Hello!" << std::endl;

    auto wT = weight_t{0.2, 0.7, 0.0, 0.1};
    auto strat = weight_alloc(wT);

    auto pT = price_t{14.5, 10.4, 11.4, 16.4, 10.5, 20.7};
    auto bt = single_asset(pT);
    auto res = bt.run(strat);

    std::cout << "CGR = " << res.cgr() << std::endl;
    std::cout << res.pvT.front() << " ... " << res.pvT.back() << std::endl;
    print_tseries(res.pvT);

    return 0;
}