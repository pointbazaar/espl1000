# Dragon

[![CodeFactor](https://www.codefactor.io/repository/github/pointbazaar/dragon/badge)](https://www.codefactor.io/repository/github/pointbazaar/dragon)

 [![Known Vulnerabilities](https://snyk.io/test/github/pointbazaar/dragon/badge.svg)](https://snyk.io/test/github/pointbazaar/dragon) 
 
!!Work in Progress!!

This repo contains a simple, work-in-progress statically typed programming language.
Its main debugger is [noBG](https://github.com/Milo-D/noBG-Assembly-Debugger)

## Basic Dependency Graph between these related repositories

- dragon depends on dragon-parser, dracovm-compiler
- dragon-parser depends on dragon-lexer

the other dependencies are just your standard maven dependencies

## Simplicity Goals
- [x] no macros (no preprocessor needed, easier to read and understand code)

## Safety Goals 
- [ ] linear/affine/... types to manage deallocations without a garbage collector and without manual memory management (like in rust)
- [x] no null/undefined/NULL/...
- [ ] possibility to do a termination proof of your program (if it is written in a special way) and termination proof friendly control structures (use foreach instead of while, use the 'loop' construct,...)
- [x] no global state
- [ ] no uninitialized variables
  - [x] no uninitialized primitives
  - [ ] no uninitialized struct members
- [ ] pattern matching 
- [ ] program verification (invariants)
- [ ] strong type system
  - [ ] Various Types of Integers
    - [x] PInt (Integer >=0) (required type for any array index)(basically one-way array bounds checking at compile time)
    - [x] NInt (Integer <=0)
    - [x] Integer (can be any integer)
    - [ ] NZInt (Integer =/= 0) (will be required as the type of any divisor later on)

## Other Goals 

- [x] simple local variable type inference 
- [x] compiles to intel x86 assembly
- [ ] 64 bit by default, 32 bit should also be supported
- [ ] functional programming (currently without closures and without some other stuff)
  - [x] functions can be passed as arguments to subroutines
  - [ ] lambda expressions
  - [ ] map, reduce, filter, sum, zip, unzip, head, tail, init
  - [x] syntax highlighting available in micro
- [x] structs (user-defined composite data types)
- [x] length-prefixed arrays
- [ ] generics (Type parameters)
- [ ] optional laziness, declare a method or function or block of statements to be lazy...
- [ ] multithreading
- [ ] standard library 
  - [ ] networking 
  - [ ] math
   - [x] abs,max,min,sign,pow
   - [ ] sin,cos,tan,...
  - [ ] string manipulations (substr)
  - [x] stdin,stdout (readchar,putchar,printi,println)
  - [ ] file system (creating files, reading files, writing files)
- [ ] support for functional style
- [ ] support for object oriented style


functional programming, imperative programming,
concurrency, laziness, declarative programming, constraint based programming,
logic programming.

it should have an easy and simple syntax, similar to java and haskell.

it should supports program verification
and programmed runtime constraints,
such as maximum time allowed for a function to run,
and can verify that on a specified computer 
with a certain clock rate and memory access latecy
some function will complete in the specified time or not.

it should support mapping of functions to integrated circuits wherever possible.
so when you make a function that accepts array type and returns another fixed size type
then this should be able to map to an abstract hardware description language.

it should automatically infer if a function has side effects or not.
the language differentiates between functions(no side effects) and 
methods(side effects). 

you can specify that a function can be run in seperate thread and cache it's 
results easily, so 5 to the power of 100 doesn't have to be computed anew all the time.

Dragon will be developed and tested against a variety of programming tasks,
from online coding katas(expressiveness,speed of execution) to hackathons(speed of development,tooling,versatility)
and business(typical business web applications, file processing) but also research programming(machine learning, artificial intelligence, rare language features,extensibility,forkability).

this should make it a practical language. goal is the ability to transfer thought into code with less code,
and enable programmer to solve tasks in new ways and think outside the box

## interesting language projects

[vlang](https://github.com/vlang/v)
[haskell](https://www.haskell.org/)
[ats](http://www.ats-lang.org/)

## Contributions

Feel free to fork the Project. 
Pull Requests are not welcome and will not be merged. 
Issues are always welcome (Suggestions for Improvement, Bug Reports, ...)
