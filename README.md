# StratoTester

Strategy Tester is a C++ header-only library for testing trading and investing strategies.

## Quick Start
Include the header as follows

```
#define STRATOTESTER_IMPL
#include <latest/StratoTester.hpp>

using namespace st;  // for convenience 
```

Here are QQQ monthly quotes from Jan 01, 2019 to Dec 01, 2019

```
auto qqq_hist = st::prices
    {168.16, 173.19, 179.66, 189.54, 173.95, 186.74,
        191.10, 187.47, 188.81, 197.08, 205.10, 212.61};
```

Let's implement a simple momentum indicator. Here, ```lambda_alloc``` is a 

```
auto last_month_up = lambda_alloc([&](prices const &price_hist) {
    // i.e. price_hist @ step 5 = [179.66, 189.54, 173.95]
    auto const last_month = price_hist.back();
    auto const first_month = price_hist.front();

    auto signal = last_month > first_month ? 1.0 : 0.0;
    std::cout << "   -> "    // logging the input and output
            << first_month << " <> " << last_month
            << " => SIG = " << signal << std::endl;
    return signal;
});
```
