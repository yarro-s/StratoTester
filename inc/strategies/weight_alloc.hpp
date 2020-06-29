#pragma once

#ifndef _HEAD_BLOCK
#include <deps.h>
#endif

#include <asset_alloc.hpp>
#include <utils.hpp>


namespace bt
{
    class weight_alloc : public asset_alloc
    {
    private:
        weight_t const &wT;

    public:
        weight algo(price_t const &pT)
        {
            size_t idxT = 
                (pT.size()-1) % wT.size();
            
            std::cout << std::endl << ">>W PT: " << ts_to_str(pT) << " => " << wT[idxT] << std::endl;
            return wT[idxT];
        }

        weight_alloc(weight_t const &wT) : wT(wT){};
    };
} // namespace bt

