#!/bin/bash

#OPTIONAL: 
#read https://medium.com/devnetwork/how-to-create-your-own-custom-terminal-commands-c5008782a78e

#STEP 1: clone this repo (master branch) to your home directory

#STEP 2: install its dependencies as described in README.MD

#STEP 3: build dragon-lexer

#STEP 4: build dragon-parser

#STEP 5: build the transpiler

#STEP 6: build the C program to invoke the transpiler
#the benefit is that we have an actual executable 
gcc -o draco draco.c
cp draco /usr/bin/draco
