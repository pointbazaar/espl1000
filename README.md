# Dragon

[![CodeFactor](https://www.codefactor.io/repository/github/pointbazaar/dragon/badge)](https://www.codefactor.io/repository/github/pointbazaar/dragon)

 [![Known Vulnerabilities](https://snyk.io/test/github/pointbazaar/dragon/badge.svg)](https://snyk.io/test/github/pointbazaar/dragon) 
 
!!Work in Progress!!

This repo contains a simple, work-in-progress statically typed programming language.

Its main debugger is [noBG](https://github.com/Milo-D/noBG-Assembly-Debugger).

The editor of choice for Dragon currently is micro, because there is a basic syntax highlighting for it.

## Basic Dependency Graph between these related repositories

- dragon depends on dragon-parser, dracovm-compiler
- dragon-parser depends on dragon-lexer

the other dependencies are just your standard maven dependencies

## Simplicity Goals

- [x] **no macros** (no preprocessor needed, easier to read and understand code)
- [x] **no backwards compatibility** (the language will change whenever a feature gets added / removed from the language. Little consideration will be given to existing code. )
- [x] Dragon has a **very simple stack based calling convention** for simplicity reasons. It is incompatible with the C calling convention.
- [x] **no void**. Every subroutine returns a value
- [x] **strict evaluation** . Dragon has no laziness, for simplicity and performance reasons.

## Safety Goals 

- [ ] **linear/affine types** to manage deallocations without a garbage collector and without manual memory management (like in rust) to get memory safety
- [x] **no null/undefined/NULL/...**
- [ ] **all programs which compile will terminate**
  - there shall be control structures which allow stuff to execute for a long time, like for web servers, command prompt loops, game loops and such, but at some point in the future it must terminate. This property is important to me. 
  - once termination proofs for suitably written programs are in the compiler, the next interesting property in that direction would be proving that it terminates in a specified timeframe (assumed unlimited memory, a given cpu clock speed, and some details)
  - [ ] possibility to do a termination proof of your program (if it is written in a special way that uses termination proof friendly control structures)
  - [ ] several termination proof friendly control structures and features
    - [x] 'loop' construct to execute a block of code a finite number of times
    - [ ] 'foreach' construct to execute a block of code for each member of an array (optionally utilize its index also)
    - [ ] ...
- [x] **no global state**
- [ ] **no uninitialized variables**
  - [x] no uninitialized primitives
  - [ ] no uninitialized struct members
- [ ] pattern matching 
- [ ] program verification using invariants
- [ ] strong type system (whatever that means?)
  - [ ] Various Types of Integers
    - [x] PInt (Integer >=0) (required type for any array index)(basically one-way array bounds checking at compile time)
    - [x] NInt (Integer <=0)
    - [x] Integer (can be any integer)
    - [ ] NZInt (Integer =/= 0) (will be required as the type of any divisor later on, to avoid division by 0)

## Other Goals 

- [ ] user defined infix,prefix,postfix functions for builtin and user defined types
  - [ ] ability to override builtin infix operators for user defined types
- [x] simple local variable type inference 
- [x] compiles to intel x86 assembly
- [ ] 64 bit by default, 32 bit should also be supported
- [ ] functional programming (currently without closures and without some other stuff)
  - [x] functions can be passed as arguments to subroutines
  - [ ] lambda expressions
  - [ ] map, reduce, filter, sum, zip, unzip, head, tail, init
  - [x] syntax highlighting available in micro
- [x] **structs** (user-defined composite data types)
- [x] **length-prefixed arrays**
- [ ] generics/templates (Type parameters)
- [ ] multithreading
- [ ] standard library with function names and signatures similar to those found in the C Standard Library
  - [ ] networking 
  - [ ] math
    - [x] abs,max,min,sign,pow
    - [ ] sin,cos,tan,...
  - [ ] string manipulations 
    - [x] substr
    - [ ] ...
  - [x] stdin,stdout (readchar,putchar,printi,println)
  - [ ] file system (creating files, reading files, writing files)
- [ ] support for functional style


functional programming, imperative programming,
concurrency, laziness, declarative programming, constraint based programming,
logic programming.

Dragon should have an easy and simple syntax, similar to C and Haskell.

Dragon should supports program verification
and programmed runtime constraints,
such as **maximum time allowed for a function to run**,
should be able to verify that on a specified computer 
with a certain clock rate and memory access latecy
some function will complete in the specified time or not.

Dragon should support mapping of functions to integrated circuits wherever possible.
so when you make a function that accepts array type and returns another fixed size type
then this should be able to map to an abstract hardware description language.


The language differentiates between functions(no side effects) and 
methods(with side effects) will enforce correct annotation of these properties.

Dragon will be developed and tested against a variety of programming tasks,
from online coding katas(expressiveness,speed of execution) to hackathons(speed of development,tooling,versatility)
and business(typical business web applications, file processing) but also research programming(machine learning, artificial intelligence, rare language features,extensibility,forkability).

## Programming Language projects that seem interesting to me

[C](https://en.wikipedia.org/wiki/C_(programming_language))
[vlang](https://github.com/vlang/v)
[haskell](https://www.haskell.org/)
[ats](http://www.ats-lang.org/)

## Contributions

Feel free to fork the Project. 
Pull Requests are not welcome and will not be merged. 
Issues are always welcome (Suggestions for Improvement, Bug Reports, ...)
