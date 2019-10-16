# Dragon

[![CodeFactor](https://www.codefactor.io/repository/github/pointbazaar/dragon/badge)](https://www.codefactor.io/repository/github/pointbazaar/dragon)
[![](https://jitpack.io/v/pointbazaar/dragon.svg)](https://jitpack.io/#pointbazaar/dragon)
 [![Known Vulnerabilities](https://snyk.io/test/github/pointbazaar/dragon/badge.svg)](https://snyk.io/test/github/pointbazaar/dragon) 
 
!!Work in Progress!!

This repo contains a simple, work-in-progress statically typed programming language.
Its main debugger is [noBG](https://github.com/Milo-D/noBG-Assembly-Debugger)

## Basic Dependency Graph between these related repositories

- dragon depends on dragon-parser, dracovm-compiler
- dragon-parser depends on dragon-lexer

the other dependencies are just your standard maven dependencies

## Goals

- [x] simple local variable type inference 
- [x] compiles to intel x86 assembly
- [ ] functional programming (currently without closures and without some other stuff)
  - [x] functions are first-class citizens and can be passed as parameters to subroutines
  - [ ] lambda expressions
  - [ ] common functions (haskell inspired) map,reduce,filter,sum,zip,unzip,head,tail,init
- [ ] structs (user-defined composite data types)
- [ ] generics (Type parameters)
- [ ] type aliases (using a different, user-defined name for a type)
- [ ] optional laziness, declare a method or function or block of statements to be lazy...
- [ ] multithreading
- [ ] standard library 
  - [ ] networking 
  - [ ] math
  - [ ] string manipulations
- [ ] support for functional style
- [ ] support for object oriented style
- [ ] pattern matching 
- [ ] program verification (invariants)
- [ ] strong type system

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

the language should allow for easy json style notation of things.

Dragon will be developed and tested against a variety of programming tasks,
from online coding katas(expressiveness,speed of execution) to hackathons(speed of development,tooling,versatility)
and business(typical business web applications, file processing) but also research programming(machine learning, artificial intelligence, rare language features,extensibility,forkability).

this should make it a practical language. goal is the ability to transfer thought into code with less code,
and enable programmer to solve tasks in new ways and think outside the box

[1,2,3,3].map(\x->x+1)

map (\x->x+1) [1,2,3,3]

