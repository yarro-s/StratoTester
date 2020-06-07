#pragma once
#include <asset_alloc.hpp>
#include <vector>

namespace bt
{
    class weight_alloc : public asset_alloc
    {
    private:
        const weight_t &wT;

    public:
        weight on_hist(const price_t &pT)
        {
            const unsigned idxT = 
                pT.size() % wT.size();
            return wT[idxT];
        }

        weight_alloc(const weight_t &wT) : wT(wT){};
        ~weight_alloc(){};
    };
} // namespace bt
