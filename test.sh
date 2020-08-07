#!/bin/bash

#STEP 1: build everything
make

#STEP 2: test dragon-lexer
cd lexer
./buildandtest.sh
cd ..

#STEP 3: test dragon-parser
cd parser
./dragon-parser -test
cd ..

#STEP 4: test the transpiler
cd transpiler
./sd -test
cd ..
