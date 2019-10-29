#!/bin/bash


#OPTIONAL: 
#read https://medium.com/devnetwork/how-to-create-your-own-custom-terminal-commands-c5008782a78e

#STEP 1: clone this repo to your home directory

#STEP 2: install its dependencies
sudo apt-get install -y nasm

#STEP 3: execute the tests and build the .jar with dependencies
./buildandtest.sh

#STEP 4: add this to your .bashrc to be able to call 'draco' from your command line
echo "source ~/dragon/.dragon.sh" >> ~/.bashrc

