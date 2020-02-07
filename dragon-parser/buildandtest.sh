#!/bin/bash

# do not use .hpp's in g++ call

make -j8

#copy the executable to /usr/bin
cp dragon-parser /usr/bin/

#dragon-parser -test