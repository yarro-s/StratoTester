#pragma once

#define _HEAD_BLOCK

/// cat templates/rel.hpp > rel.hpp
/// ./make_header.sh
/// g++ -P -E rel.hpp -o rel_prep.hpp -I../inc -I../inc/strategies


///     cat ../inc/result.hpp >> rel.hpp ....
/// cat ../inc/strategies/*.hpp >> rel.hpp
/// cat ../inc/*.hpp >> rel.hpp




#include <asset_alloc.hpp>
#include <backtest.hpp>
#include <balance_book.hpp>
#include <result.hpp>
#include <single_asset.hpp>
#include <t_series.hpp>
#include <unit_defs.hpp>
#include <utils.hpp>
#include <ma_cross.hpp>
#include <random_alloc.hpp>
#include <weight_alloc.hpp>
