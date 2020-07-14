#!/bin/bash

#OPTIONAL: 
#read https://medium.com/devnetwork/how-to-create-your-own-custom-terminal-commands-c5008782a78e

#STEP 1: clone this repo (master branch) to your home directory

#STEP 2: install its dependencies as described in README.MD

#STEP 3: build dragon-lexer
cd dragon-lexer/src
sudo ./build.sh
cd ../..

#STEP 4: build dragon-parser
cd dragon-parser
sudo ./build.sh
cd ..

#STEP 5: build the transpiler
cd transpiler
sudo ./buildandtest.sh
cd ..
