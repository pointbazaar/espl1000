#!/bin/bash

#this script should be run with sudo

rm /usr/bin/smalldragon
rm smalldragon

make -j8
make clean

#copy the executable to /usr/bin
cp smalldragon /usr/bin/

smalldragon -test -debug
