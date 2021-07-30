<img src="https://raw.githubusercontent.com/pointbazaar/smalldragon/master/docs/img/dragon-logo.svg" width="100" height="100"/>

# SmallDragon, a simple Programming Language [WIP] 
[![Build Status](https://travis-ci.com/pointbazaar/smalldragon.svg?branch=master)](https://travis-ci.com/pointbazaar/smalldragon)
[![CodeFactor](https://www.codefactor.io/repository/github/pointbazaar/smalldragon/badge)](https://www.codefactor.io/repository/github/pointbazaar/smalldragon)
[![GitHub license](https://img.shields.io/github/license/pointbazaar/smalldragon.svg)](https://github.com/pointbazaar/smalldragon/blob/master/LICENSE)
[![Open Source Love png2](https://badges.frapsoft.com/os/v2/open-source.png?v=103)](https://github.com/ellerbrock/open-source-badges/)
 
This repo contains a simple, statically typed programming language that can be transpiled to C.

## Description

smalldragon is inspired by the simplicity and power of C programming.

It aims to be a modern language which captures the unique feeling of C programming, 
but in a slightly different way.

One of that goals is that the programmer can focus and does not have to look at documentation all the time.

## Documentation
### [smalldragon.org](https://smalldragon.org) 

## How to build it

This Project will probably only work with Linux.

It has been tested/is being developed on Ubuntu 20.04.

0. ```git clone git@github.com:pointbazaar/smalldragon.git```
1. ```sudo apt-get install -y gcc cmake make valgrind flex```
2. Add 
   ```PATH=~/smalldragon/transpiler:~/smalldragon/lexer:~/smalldragon/parser:$PATH```
   to your ```~/.bashrc``` so that the binaries can be found.
3. ```make```
