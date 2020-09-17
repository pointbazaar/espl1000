<img src="https://raw.githubusercontent.com/pointbazaar/smalldragon/dev/lexer/img/dragon-logo.svg" width="100" height="100"/>

# SmallDragon, a simple Programming Language (Work in Progress) 
master: [![Build Status](https://travis-ci.org/pointbazaar/smalldragon.svg?branch=master)](https://travis-ci.org/pointbazaar/smalldragon)
dev: [![Build Status](https://travis-ci.org/pointbazaar/smalldragon.svg?branch=dev)](https://travis-ci.org/pointbazaar/smalldragon)

[![CodeFactor](https://www.codefactor.io/repository/github/pointbazaar/smalldragon/badge)](https://www.codefactor.io/repository/github/pointbazaar/smalldragon)
[![GitHub license](https://img.shields.io/github/license/pointbazaar/smalldragon.svg)](https://github.com/pointbazaar/smalldragon/blob/master/LICENSE)
[![Open Source Love png2](https://badges.frapsoft.com/os/v2/open-source.png?v=103)](https://github.com/ellerbrock/open-source-badges/)
 
This repo contains a simple, work-in-progress statically typed programming language that aims to be able to be transpiled to C.

The Goal is to have a language to make it easier to implement Open Source Projects in C, 
and allow writing some parts of the Project in a higher-level language such as SmallDragon.

The Goal is for the Experience to be similar to working with C and Assembly. 
There you have more control of implementation details with Assembly, but more abstraction and productivity with C.

That's the Vision for SmallDragon, to be more abstract than C, but to be able to work closely with it by transpiling to it. 
SmallDragon should be a clean abstraction over C.

The goal is to actually **understand** what is going on, meaning that if you understand **C**, then you can easily understand 
**smalldragon** and leverage your existing understanding of C. Many languages of a level higher than C do not work
on top of it, but based on entirely different abstractions, like a VM or an Interpreter. It is often very difficult to understand
what is actually going on at runtime, even if the semantics of the language are very simple. 



There is currently no plan to have a library for SmallDragon, besides some small utility files in C, to get some basic abstractions going.

The editor of choice for smalldragon currently is [micro](https://micro-editor.github.io/), because there is a basic syntax highlighting for it.

## Examples
Have a look at [Examples](https://github.com/pointbazaar/smalldragon/tree/dev/examples)

```console
You@Terminal:~$ sd Main.dg
You@Terminal:~$ ./main
```

[SmallDragon's Architecture](https://raw.githubusercontent.com/pointbazaar/smalldragon/dev/architecture.svg)

## How to install / use this Project ?

This Project will probably only work with Linux.

0. install gcc, make 
1. try compiling some sample program

## Simplicity

- [x] **hackable to the core**. Make it your own. Everything in SmallDragon is created with extendability and modularity in Mind. The Transpiler is composed of many small programs, written in different language, which communicate via easy-to-understand file formats and protocols. 
- [x] **no macros** (no preprocessor needed, easier to read and understand code)
- [x] **no backwards compatibility** (the language will change whenever a feature gets added / removed from the language. Little consideration will be given to existing code. )
- [x] **no void**. Every subroutine returns a value
- [x] **strict evaluation** . SmallDragon has no laziness, for simplicity and performance reasons.
- [x] no user defined prefix , infix or suffix operators. This simplifies the parser.
- [x] this is **not a self hosting compiler**. this is for stability reasons. also, other languages are great too. i think it's a bit narcissitic to have a self-hosting compiler.
- [x] no aliasing of anything in general. this improves readability. 
- [x] no need to declare a namespace. the filename is the name of the namespace. subroutine and struct declarations are at the first level of indentation.
- [ ] simple syntax. access modifiers (public, private, ...) and such can be defined (might be implemented later) in a .dg.json file. So don't need to build a parser for that, can just use a simple json parser. The .dg.json file can also be used to list usages of other modules "imports" and such, but also usages of 'extern' functions from assembly or c or anything really. It is meant for any bigger project which benefits from a more structured programming approach, where visibilites of subroutines, structs and moules actually makes sense.


## Safety Goals 

- [x] **no null/undefined/NULL/...**
- [ ] **termination proof for suitably written programs/functions**
  - there shall be control structures which allow stuff to execute for a long time, like for web servers, command prompt loops, game loops and such, but at some point in the future it must terminate. This property is important to me. 
  - once termination proofs for suitably written programs are in the compiler, the next interesting property in that direction would be proving that it terminates in a specified timeframe (assumed unlimited memory, a given cpu clock speed, and some details)
  - [ ] possibility to do a termination proof of your program (if it is written in a special way that uses termination proof friendly control structures)
  - [ ] several termination proof friendly control structures and features
    - [x] 'loop' construct to execute a block of code a finite number of times
    - [ ] 'for in ' construct to execute a block of code for an index in some range
- [x] **no global state**
- [ ] **no uninitialized variables**
  - [x] no uninitialized primitives
  - [ ] no uninitialized struct members
- [ ] pattern matching 
- [ ] program verification using invariants
- [ ] strong type system (whatever that means?)
  - [ ] Various Types of Integers
    - [ ] PInt (Integer >=0) (required type for any array index)(basically one-way array bounds checking at compile time)
    - [ ] NInt (Integer <=0)
    - [ ] Int (can be any integer)
    - [ ] NZInt (Integer =/= 0) (will be required as the type of any divisor later on, to avoid division by 0)

- [ ] lambda expressions
- [x] syntax highlighting available in micro

smalldragon should have an easy and simple syntax, similar to C and Haskell.

smalldragon should supports program verification.
The language differentiates between functions(no side effects) and 
methods(with side effects) will enforce correct annotation of these properties.

It is intended to be used for (but not limited to):

- writing a Program in SmallDragon and transpiling it to C.
- writing a part of your Program in C and another part in SmallDragon.

## Contributions

- Feel free to fork the Project. 
- Pull Requests are not welcome and will not be merged. 
- Issues are always welcome (Suggestions for Improvement, Bug Reports, ...)
