#pragma once
#include <unit_defs.hpp>
#include <math.h>

namespace bt
{
    class BTResult
    {
    public:
        const price_t &pvT;

    public:
        double growth()
        {
            return pvT.back() / pvT.front();
        }
        double cgr()
        {
            return pow(growth(), pvT.size()) - 1;
        }
        BTResult(price_t &pvT) : pvT(pvT) {}
        ~BTResult() {}
    };
} // namespace bt
