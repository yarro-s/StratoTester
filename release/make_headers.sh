#!/bin/sh

HEADER=rel.hpp
for file in ../inc/*.hpp
do
    echo "#include <${file##*/}>" >> $HEADER
done

for file in ../inc/strategies/*.hpp
do
    echo "#include <${file##*/}>" >> $HEADER
done
