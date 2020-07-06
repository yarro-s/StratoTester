// Copyright 2020 Yarro S

#pragma once

#include <unit_defs.hpp>


namespace bt {

class trader {
 public:
    /// TODO: make polymorphic
    virtual trader &update(price px, weight w) = 0;
};
}  // namespace bt
