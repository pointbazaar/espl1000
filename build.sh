#!/bin/bash

#STEP 1: build dragon-lexer
cd lexer
sudo ./build.sh
cd ..

#STEP 2: build dragon-parser
cd parser
sudo ./build.sh
cd ..

#STEP 3: build the transpiler
cd transpiler
sudo ./build.sh
cd ..
