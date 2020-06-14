#pragma once

#include <vector>
#include <asset_alloc.hpp>

#include <iostream>
#include <utils.hpp>

namespace bt
{
    class weight_alloc : public asset_alloc
    {
    private:
        const weight_t &wT;

    public:
        weight algo(const price_t &pT)
        {
            size_t idxT = 
                (pT.size()-1) % wT.size();

            // std::cout << std::endl << ">>PT: " << ts_to_str(pT) << " => " << wT[idxT] << std::endl;
            return wT[idxT];
        }

        weight_alloc(const weight_t &wT) : wT(wT){};
        ~weight_alloc(){};
    };
} // namespace bt
