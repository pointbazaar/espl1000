# Dragon

!! Work in Progress !!

This repo contains a simple, work-in-progress statically typed programming language.

## Goals

- [x] simple local variable type inference 
- [x] compiles to intel x86 assembly
- [x] syntax highlighting available in Micro
- [ ] 64 bit only, to enable 64 bit primitives by default
- [ ] functional programming
  - [x] functions can be passed as parameters
  - [ ] lambda expressions
  - [ ] common functions (haskell inspired) map,reduce,filter,sum,zip,unzip,head,tail,init
- [ ] structs
- [ ] objects
  - [ ] support for creating objects
  - [ ] generics (Type parameters)
  - [ ] type aliases
- [ ] optional laziness, declare a method or function or block of statements to be lazy...
- [ ] multithreading
- [ ] standard library 
  - [ ] networking 
  - [x] stdin,stdout (readchar,putchar,printi,println)
  - [ ] file system (creating files, reading files, writing files)
  - [x] math (min,max,faculty,pow,sign)
  - [x] string manipulations (substr)
- [ ] support for functional style
- [ ] support for object oriented style
- [ ] pattern matching 
- [ ] program verification (invariants)
- [ ] strong type system
  - [ ] Various Types of Integers
    - [x] PInt (Integer >=0) (required type for any array index)
    - [x] NInt (Integer <=0)
    - [x] Integer (can be any integer)
    - [ ] NZInt (Integer =/= 0) (will be required as the type of any divisor later on)

object oriented programming, functional programming, imperative programming,
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

