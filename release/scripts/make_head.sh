#!/bin/sh

HEADER=backtester_head.hpp
for file in ../inc/*.hpp
do
    echo "#include <${file##*/}>" >> $HEADER
done

for file in ../inc/allocators/*.hpp
do
    echo "#include <${file##*/}>" >> $HEADER
done
