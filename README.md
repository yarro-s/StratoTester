# StratoTester

Strategy Tester is a C++ header-only library for testing trading and investing strategies.

## Quick Start
Include [StratoTester.hpp](https://github.com/yarro-s/StratoTester/tree/master/release/latest) as follows

```C++
#define STRATOTESTER_IMPL  // specified only once 
#include <StratoTester.hpp>

using namespace st;  // for convenience 
```

Here are some QQQ monthly quotes, starting from Jan 01, 2019 ending Dec 01, 2019

```C++
auto qqq_hist = st::prices {168.16, 173.19, 179.66, 189.54, 173.95, 186.74,
                            191.10, 187.47, 188.81, 197.08, 205.10, 212.61};
```

Let's implement a simple momentum trading rule 

```C++
auto last_close_up = [&](prices const &price_hist) {
    auto const last_close = price_hist.back();
    auto const first_close = price_hist.front();

    // buy when last close is higer than the first, hold otherwise
    return last_close > first_close ? 1.0 : 0.0;
};
```

Next, let's create a strategy based on the rule

```C++
auto strat = strategy::with(last_close_up)
                .look_back(3)         // 3 samples rolling window
                .rebalance_every(2);  // rebanace every 2 samples
```

We specified a 3 sample rolling window with ```look_back(3)``` with the asset rebalancing done every 2 samples. The samples hold the monthly closing prices of QQQ.

Let's run this strategy with an initial deposit of $10000

```C++
auto init_deposit = 10000;
auto test = st::single_asset(strat, init_deposit).run(qqq_hist);
```

and log some of the results

```C++
std::cout << std::endl
                  << "Single asset portfolio value history: " << std::endl
                  << "   " << str_rep(test.results().value_history())
                  << std::endl;
std::cout << std::endl
            << "Single asset portfolio total return: "
            << test.results().total_return() << std::endl;
```



