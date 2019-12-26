#!/bin/bash


#OPTIONAL: 
#read https://medium.com/devnetwork/how-to-create-your-own-custom-terminal-commands-c5008782a78e

#STEP 1: clone this repo to your home directory

#STEP 2: install its dependencies
sudo apt-get install -y nasm

#STEP 3: execute the tests and build the .jar with dependencies
./buildandtest.sh


#STEP 4: build the C program to invoke the java program
# the benefit is that we have an actual executable 
gcc -o draco draco.c
cp draco /usr/bin/draco
