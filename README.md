# fortisquid
fortified squid. or fortunate squid. programming language (just an idea now) that aims to integrate multiple programming concepts

such as object oriented programming, functional programming, imperative programming,
concurrency, laziness, declarative programming, constraint based programming,
logic programming.

GOALS:

it should have an easy and simple syntax, similar to javascript, java, haskell.

it supports program verification
and programmed runtime constraints,
such as maximum time allowed for a function to run,
and can verify that on a specified computer 
with a certain clock rate and memory access latecy
some function will complete in the specified time or not.

great for embedded programming.

it should support mapping of functions to integrated circuits wherever possible.
so when you make a function that accepts array type and returns another fixed size type
then this should be able to map to an abstract hardware description language.

it should automatically infer if a function has side effects or not.
the language differentiates between functions(no side effects) and 
methods(side effects). 

you can specify that a function can be run in seperate thread and cache it's 
results easily, so 5 to the power of 100 doesn't have to be computed anew all the time.

the language should allow for easy json style notation of things
and should 


fortisquid should compile to C,java bytecode(to run on the jvm),java,javascript(to run in the browser),
and arm assembly language to demonstrate runtime guarantees on raspberry pi

fortisquid should also have tooling to be able to compile java,javascript to fortisquid
to easy any migration you might want to do.

fortisquid will be developed and tested against a variety of programming tasks,
from online coding katas(expressiveness,speed of execution) to hackathons(speed of development,tooling,versatility)
and business(typical business web applications, file processing) but also research programming(machine learning, artificial intelligence, rare language features,extensibility,forkability).

this should make it a practical language. goal is the ability to transfer thought into code with less code,
and enable programmer to solve tasks in new ways and think outside the box
