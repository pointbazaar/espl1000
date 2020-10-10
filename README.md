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

smalldragon should have an easy and simple syntax, similar to C and Haskell.

It is intended to be used for (but not limited to):

- writing a Program in SmallDragon and transpiling it to C.
- writing a part of your Program in C and another part in SmallDragon.

## Contributions

- Feel free to fork the Project. 
- Pull Requests are not welcome and will not be merged. 
- Issues are always welcome (Suggestions for Improvement, Bug Reports, ...)
