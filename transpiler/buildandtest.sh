#!/bin/bash

#this script should be run with sudo

rm sdg
rm /usr/bin/sdg

make -j8
make clean

#copy the executable to /usr/bin
cp sdg /usr/bin/

sdg -test -debug
