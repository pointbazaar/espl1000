# Dragon (work in progress)
[![Build Status](https://travis-ci.org/pointbazaar/dragon.svg?branch=master)](https://travis-ci.org/pointbazaar/dragon)
[![CodeFactor](https://www.codefactor.io/repository/github/pointbazaar/dragon/badge)](https://www.codefactor.io/repository/github/pointbazaar/dragon)
 [![Known Vulnerabilities](https://snyk.io/test/github/pointbazaar/dragon/badge.svg)](https://snyk.io/test/github/pointbazaar/dragon) 
 [![GitHub license](https://img.shields.io/github/license/pointbazaar/dragon.svg)](https://github.com/pointbazaar/dragon/blob/master/LICENSE)
 [![GitHub issues](https://img.shields.io/github/issues/pointbazaar/dragon.svg)](https://GitHub.com/pointbazaar/dragon/issues/)
 [![Average time to resolve an issue](http://isitmaintained.com/badge/resolution/pointbazaar/badges.svg)](http://isitmaintained.com/project/pointbazaar/badges "Average time to resolve an issue")
 
This repo contains a simple, work-in-progress statically typed programming language.

Its main debugger is [noBG](https://github.com/Milo-D/noBG-Assembly-Debugger).
The editor of choice for Dragon currently is [micro](https://micro-editor.github.io/), because there is a basic syntax highlighting for it.

## Usage

Basic Program:

```dragon
struct Point{
  PInt x,
  PInt y
}


fn main :: () ~> PInt
fn main () {
  //every variable is always initialized
  Point p = (1,2);  //structs can also be interpreted as tuples
  return subr(1,p.y);
}

fn subr :: (PInt,PInt) -> PInt
fn subr (a,b) {
  z=b+1;  //local variable type inference
  return a+z;
}
```

```console
You@Terminal:~$ draco Main.dg
```

## Basic Dependency Graph between these related repositories

- dragon depends on dragon-parser, dracovm-compiler
- dragon-parser depends on dragon-lexer

the other dependencies are just your standard maven dependencies

## Simplicity

- [x] **no macros** (no preprocessor needed, easier to read and understand code)
- [x] **no backwards compatibility** (the language will change whenever a feature gets added / removed from the language. Little consideration will be given to existing code. )
- [x] Dragon has a **very simple stack based calling convention** for simplicity reasons. It is incompatible with the C calling convention.
- [x] **no void**. Every subroutine returns a value
- [x] **strict evaluation** . Dragon has no laziness, for simplicity and performance reasons.
- [x] no user defined prefix , infix or suffix operators. This simplifies the parser.
- [x] this is **not a self hosting compiler**. this is for stability reasons. also, other languages are great too. i think it's a bit narcissitic to have a self-hosting compiler.
- [x] no aliasing of anything in general. this improves readability. 
- [x] no need to declare a namespace. the filename is the name of the namespace. subroutine and struct declarations are at the first level of indentation.
- [ ] simple syntax. access modifiers (public, private, ...) and such can be defined (might be implemented later) in a .dg.json file. So don't need to build a parser for that, can just use a simple json parser. The .dg.json file can also be used to list usages of other modules "imports" and such, but also usages of 'extern' functions from assembly or c or anything really. It is meant for any bigger project which benefits from a more structured programming approach, where visibilites of subroutines, structs and moules actually makes sense.
But the .dg.json file is optional and does not obstruct the simplicity of the .dg file.

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
concurrency, laziness, declarative programming, constraint based programming.

Dragon should have an easy and simple syntax, similar to C and Haskell.

Dragon should supports program verification
and programmed runtime constraints,
such as **maximum time allowed for a function to run**,
should be able to verify that on a specified computer 
with a certain clock rate and memory access latecy
some function will complete in the specified time or not.

The language differentiates between functions(no side effects) and 
methods(with side effects) will enforce correct annotation of these properties.

Dragon will be developed and tested against a variety of programming tasks.

## Programming Language projects that seem interesting to me

[C](https://en.wikipedia.org/wiki/C_(programming_language))
[vlang](https://github.com/vlang/v)
[haskell](https://www.haskell.org/)
[ats](http://www.ats-lang.org/)

## Interesting websites related to programming languages

[lambda the ultimate](http://lambda-the-ultimate.org/)

## Contributions

- Feel free to fork the Project. 
- Pull Requests are not welcome and will not be merged. 
- Issues are always welcome (Suggestions for Improvement, Bug Reports, ...)
