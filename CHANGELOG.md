#Changelog
All notable changes to this project will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/)

## [Unreleased]
- remove the AST I/O feature (the .file.dg.ast file format)
  because i did not use the feature. It was intended to have a common format
  to interact with the AST, for multiple programs to work on it. 
  But it is way easier to just link the lexer and parser into the programs
  which want to interact with the AST. It is simply not adding much to the compiler 
  in terms of functionality and because of that i'm getting rid of `ast/io`.
  It was too much work to maintain the serialization/deserialization of every AST node, 
  which had to be changed whenever the syntax changed or the AST representation changed.

## [v0.3.1]
- the arrows in the .dot file export for basic blocks now have colors {true:green, false:red}
- removed the C backend (except the forward declarations, for C interop)
- Added 'i++', 'i--', '%=' syntactic sugar
- removed Exception ('throws') and try-catch from smalldragon. This simplifies the controlflow, 
  as there are now no stackframes being skipped and cleanup logic inside the function 
  can be executed. This may simplify features to be implemented later on, such as 
  mechanisms for structured concurrency and memory safety.

## [v0.3.0]
- implement a simple Typechecker
- implement '-h' flag to emit a .h (header) file
- implement basic Lambda Support
- implement basic Annotation Support, including @halts, @deprecated, @private
- implement Struct Field Reordering to minimize padding

## [v0.2.1]
- Dead Code Analysis is now supported. For now, this does not 
  include structures.
- A very simple Termination Analysis is now possible. 

## [v0.2.0]
- Analyze Callers and Callees of subroutines
- Introduce AST Nodes 'PrimitiveType', 'StructType' as members of
  'SimpleType'
- smalldragon now supports transpiling multiple .dg files into one .c file.
  This is the first step in the foundation for building larger programs
  in smalldragon.

## [v0.1.5]
- Added new Assignment Operators: &=, |=, >>=, <<=
- remove '-stdout' flag
- separate the local variable analyzer from the LVST into lv_analyzer.c
- make several libC function signatures available for smalldragon type inference. 

## [v0.1.4]
- Added Support for 'continue' Statement.

## [v0.1.3]
- Create more directories to have a better Structure in the Project
- Typedef some Structures
- Adjust most .c files to have a lowercase filename
- Refactor struct Variable 

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
