#!/bin/bash

#STEP 1: build everything
./build.sh

#STEP 2: test dragon-lexer
cd lexer
./buildandtest.sh
cd ..

#STEP 3: test dragon-parser
cd parser
./buildandtest.sh
cd ..

#STEP 4: test the transpiler
cd transpiler
./buildandtest.sh
cd ..
