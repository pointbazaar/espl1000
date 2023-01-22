<img src="https://raw.githubusercontent.com/pointbazaar/espl1000/master/docs/img/dragon-logo.svg" width="100" height="100"/>

# ESPL1000 [WIP]
[![GitHub license](https://img.shields.io/github/license/pointbazaar/espl1000.svg)](https://github.com/pointbazaar/espl1000/blob/master/LICENSE)
[![Open Source Love png2](https://badges.frapsoft.com/os/v2/open-source.png?v=103)](https://github.com/ellerbrock/open-source-badges/)

## CI Status
[![Build Status](https://app.travis-ci.com/pointbazaar/espl1000.svg?branch=master)](https://app.travis-ci.com/github/pointbazaar/espl1000)
[![CircleCI](https://dl.circleci.com/status-badge/img/gh/pointbazaar/espl1000/tree/master.svg?style=svg)](https://dl.circleci.com/status-badge/redirect/gh/pointbazaar/espl1000/tree/master)
[![CodeFactor](https://www.codefactor.io/repository/github/pointbazaar/espl1000/badge)](https://www.codefactor.io/repository/github/pointbazaar/espl1000)


This repo contains a simple, statically typed programming language.
The Compiler Backend targets the ATMEL(now Microchip) ATMega328P.

## Description

ESPL1000 is inspired by the simplicity and power of C programming.

The Compiler is Work in Progress and still a long way from being functional.

## Documentation
### [espl1000.org](https://espl1000.org)

## How to build it

## Build Instructions (other Linux)

0. clone the repository
1. get required packages via your distributions package manager
   which would be:
   gcc cmake make valgrind flex avra
2. modify your $PATH environment variable to include
   lexer/build , parser/build and compiler/ directories.
3. make

## Build Instructions (Debian/Ubuntu)

It has been tested/is being developed on Ubuntu 20.04.

0. ```git clone git@github.com:pointbazaar/espl1000.git```
1. ```sudo apt-get install -y gcc cmake make valgrind flex avra```
2. Add
   ```PATH=~/espl1000/compiler/build:~/espl1000/lexer/build:~/espl1000/parser/build:$PATH```
   to your ```~/.bashrc``` so that the binaries can be found.
3. ```make```
