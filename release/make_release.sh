#!/bin/sh

g++ -E -P all_modules.hpp -I../inc -I../src > all_modules_prep.hpp
cat ../inc/deps.hpp all_modules_prep.hpp >> backtest.hpp

rm all_modules_prep.hpp