#!/bin/bash

#this script should be run with sudo

rm sd
rm /usr/bin/sd

make -j8
make clean

#copy the executable to /usr/bin
cp sd /usr/bin/

sd -test -debug
