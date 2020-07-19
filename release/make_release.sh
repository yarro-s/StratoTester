#!/bin/sh

HEADER=backtester_head.hpp
SOURCE=backtester.cpp
RELEASE=latest/backtester.hpp

cat templates/$HEADER > ${HEADER}
cat templates/$SOURCE > ${SOURCE}

for file in $(find ../inc -name '*.hpp')
do
    echo "#include \"${file}\"" >> $HEADER
done

cat ../src/*.cpp >> $SOURCE

# TODO: fix the ugly includes
g++ -P -E $HEADER -o _head_prep.hpp -I. -I../inc
g++ -P -E $SOURCE -o _backtester_prep.cpp

cat inc/deps.hpp > $RELEASE
cat _head_prep.hpp >> $RELEASE

echo "#ifdef BACKTESTER_IMPL" >> $RELEASE
cat _backtester_prep.cpp >> $RELEASE
echo "#endif" >> $RELEASE

# clean up
rm *pp