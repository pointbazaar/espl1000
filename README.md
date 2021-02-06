<img src="https://raw.githubusercontent.com/pointbazaar/smalldragon/dev/docs/img/dragon-logo.svg" width="100" height="100"/>

# SmallDragon, a simple Programming Language (Work in Progress) 
master: [![Build Status](https://travis-ci.org/pointbazaar/smalldragon.svg?branch=master)](https://travis-ci.org/pointbazaar/smalldragon)
dev: [![Build Status](https://travis-ci.org/pointbazaar/smalldragon.svg?branch=dev)](https://travis-ci.org/pointbazaar/smalldragon)


[![CodeFactor](https://www.codefactor.io/repository/github/pointbazaar/smalldragon/badge)](https://www.codefactor.io/repository/github/pointbazaar/smalldragon)
[![GitHub license](https://img.shields.io/github/license/pointbazaar/smalldragon.svg)](https://github.com/pointbazaar/smalldragon/blob/master/LICENSE)
[![Open Source Love png2](https://badges.frapsoft.com/os/v2/open-source.png?v=103)](https://github.com/ellerbrock/open-source-badges/)
 
This repo contains a simple, work-in-progress statically typed programming language that aims to be able to be transpiled to C.

The Goal is for the Experience to be similar to working with C and Assembly. 
There you have more control of implementation details with Assembly, but more abstraction and productivity with C.

That's the Vision for SmallDragon, to be more abstract than C, but to be able to work closely with it by transpiling to it. 
SmallDragon should be a clean abstraction over C.

The goal is to actually **understand** what is going on, meaning that if you understand **C**, then you can easily understand 
**smalldragon** and leverage your existing understanding of C. Many languages of a level higher than C do not work
on top of it, but based on entirely different abstractions, like a VM or an Interpreter. It is often very difficult to understand
what is actually going on at runtime, even if the semantics of the language are very simple. 


The editor of choice for smalldragon currently is [micro](https://micro-editor.github.io/), because there is a basic syntax highlighting for it.

## Examples
Have a look at [Examples](https://github.com/pointbazaar/smalldragon/tree/dev/examples)

```console
You@Terminal:~$ sd Main.dg
You@Terminal:~$ ./main
```

[SmallDragon's Architecture](https://raw.githubusercontent.com/pointbazaar/smalldragon/dev/docs/architecture.svg)

[Documentation](https://smalldragon.org)

## How to install / use this Project ?

This Project will probably only work with Linux.
It has been tested/is being developed on Ubuntu 20.04.
There should be a test matrix with multiple distros soon hopefully.

0. Install gcc, gcc-10, make, valgrind 
1. Try compiling some sample program
2. If you want the binaries to be visible globally:
   Add 
   ```PATH=~/smalldragon/transpiler:~/smalldragon/lexer:~/smalldragon/parser:$PATH```
   to your ~/.bashrc

## Contributions

- Feel free to fork the Project. 
- Pull Requests will not be merged, as this is a learning Project.
- Issues are always welcome (Suggestions for Improvement, Bug Reports, ...)
