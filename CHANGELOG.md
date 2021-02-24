#Changelog
All notable changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)

## [Unreleased]
- Create more directories to have a better Structure in the Project
- Typedef some Structures
- Adjust most .c files to have a lowercase filename

## [0.1.2]
- Change the names of primitive integer types. (e.g. use int32 now instead of Int32)
  which is easier to write. New Names: [char, float, bool, int, uint, ... (various int types)] 

## [0.1.1] - 2021-02-20
- [x] Use a binary format (utilizing fread, fwrite) for the .ast files.
  This should increase performance, as struct members can be directly
  initialized from file contents. No checking/conversion is neccessary.
  The reader and writer for .ast format use magic numbers internally
  for debugging purposes and to detect broken .ast files.

## [0.1.0] - 2021-02-15
- Use a GNU Flex based Lexer implementation instead of a handwritten Lexer
- Add Support for Hex Literals    (prefix 0x) in smalldragon code
- Add Support for Binary Literals (prefix 0b) in smalldragon code

## [0.0.6] - 2020-09-16
- Added '-stdout' flag to print transpiled C code to stdout
- Updated SmallDragon Syntax Highlighting for Vim 
- Added Comparison Operator Chaining
- Added '+=','-=','\*=','/=' Assignment Operators
- Added '&','|','~','^','<<','>>' Bitwise Operators 

## [0.0.5] - 2020-09-12
- Added '..' (the Range Operator) which can be used to interpolate between Integers
- Added a For-style Statement for iterating, e.g. 'for i in 0..10 { /*do stuff*/ }'
- Added Support for "functional style" Programming by transpiling to C function pointers 
- Added a Switch-style Statement

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
