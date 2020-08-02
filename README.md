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
auto qqq_hist = st::prices
    {168.16, 173.19, 179.66, 189.54, 173.95, 186.74,
        191.10, 187.47, 188.81, 197.08, 205.10, 212.61};
```

Let's implement a simple momentum indicator. Here, ```lambda_alloc``` is an allocator wrapper around a lambda function, which returns the weight of QQQ in our portfolio based on its recent price history 

```C++
auto last_month_up = lambda_alloc([&](prices const &price_hist) {
    auto const last_month = price_hist.back();
    auto const first_month = price_hist.front();

    auto signal = last_month > first_month ? 1.0 : 0.0;
    std::cout << "   -> "    // logging the input and output
            << first_month << " <> " << last_month
            << " => SIG = " << signal << std::endl;
    return signal;
});
```

Next, let's create a strategy based on the allocator

```C++
auto strat = strategy(&last_month_up)
            .look_back(3)           // 3 samples rolling window
            .rebalance_every(2);    // rebanace every 2 months
```

We specified a 3 samples rolling window with ```look_back(3)``` with the asset rebalancing done every 2 samples. Here, the samples hold the monthly closing price of QQQ.

Let's run this strategy with some money

```C++
auto init_deposit = 10000;
auto test = st::single_asset(strat, init_deposit).run(qqq_hist);
```

and log some of the results

```C++
std::cout << std::endl <<
        "Single asset portfolio value history: " << std::endl <<
        "   " <<
        str_rep(test.results().value_history()) << std::endl;
std::cout << std::endl <<
        "Single asset portfolio total return: " <<
        test.results().total_return() << std::endl;
```



