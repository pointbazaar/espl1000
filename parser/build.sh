#!/bin/bash

#this script should be run with sudo

rm /usr/bin/dragon-parser
rm dragon-parser

make -j8 all

#copy the executable to /usr/bin
cp dragon-parser /usr/bin/

