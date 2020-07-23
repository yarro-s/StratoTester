#!/bin/sh

cd build
cmake ..

SHOWCASE=BTLightMain

rm $SHOWCASE ; cmake --build . ; ./$SHOWCASE