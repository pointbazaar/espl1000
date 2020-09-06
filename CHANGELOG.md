#Changelog
All notable changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)

## [Unreleased]
- Added '..' (the Range Operator) which can be used to interpolate between Integers
- Added a For-style Statement for iterating, e.g. 'for i in 0..10 { /*do stuff*/ }'

## [0.0.4] - 2020-09-6
- Added 'sd -help' Option
- Added 'sd -version' Option
- Added '-clean' Flag to dragon-lexer
- Added '-clean' Flag to sd (in transpiler/)
- Added Valgrind Tests to detect Memory Leaks
- Added Local Variable Type Inference
- Added special subroutines which are automatically generated
  to handle allocate, copy, free for structures

## [0.0.3] - 2020-08-27
- Added 'break' Statement

## [0.0.2] - 2020-08-27
- Added integration tests in transpiler/test/suite/
- Added 'loop' Statement back in again
- "make" now also runs the tests after it builds.
- Removed test.sh

## [0.0.1] - 2020-08-19
- started using CHANGELOG.md
- Added lexer/ 
- Added parser/ 
- Added transpiler/
- Added syntax-tools/ 
- Added examples/
- Added util/ 
- Added spec/
- Added HOWTOINSTALL.txt
- Added LICENSE
