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

Let's implement a simple momentum trading rule:  

```C++
auto last_close_up = [=](prices const &price_hist) {
    auto const last_close = price_hist.back();
    auto const first_close = price_hist.front();

    // buy when last close is higer than the first, hold otherwise
    return last_close > first_close ? 1.0 : 0.0;
};
```

A trading rule takes a rolling price history and calculates ```weight``` in the range from ```-1.0``` to ```1.0```. For instance, returning ```0.5``` means that the currently required share  of the asset in the portfolio is 50% while returning ```-0.2``` means that 20% of the portfolio is used for shorting the asset.

Next, let's create a strategy based on that rule

```C++
auto strat = strategy::with(last_close_up)
                .look_back(3)         // 3 samples rolling window
                .rebalance_every(2);  // rebalanace every 2 samples
```

We specified a three-sample rolling window with ```look_back(3)```. Asset rebalancing, specified as ```rebalance_every(2)```, is done every two samples. The samples here hold the monthly closing prices of QQQ for the year 2019.

Let's run this strategy with an initial deposit of $10000

```C++
auto init_deposit = 10000;
auto test = st::single_asset(strat, init_deposit).run(qqq_hist);
```

and log some of the results

```C++
std::cout << "Single asset portfolio value history: " << std::endl
          << str_rep(test.results().value_history())
          << std::endl;
std::cout << "Single asset portfolio total return: "
          << test.results().total_return() << std::endl;
```

After requesting the backtesting results with ```results()```, you can access all the historical values of the portfolio with ```value_history()``` and calculate its total return with ```total_return()```. 



