
all: build test

#https://cliutils.gitlab.io/modern-cmake/chapters/intro/running.html

#older versions of CMake sadly do not support --parallel option

build: DIRS
	cmake -S . -B build -H.
	cmake --build build --parallel

DIRS:
	mkdir -p build/

test: build/dragon-parser
	./build/dragon-parser-tests

clean: DIRS
	rm -rf build/*
	rm -f dragon-parser

VALGRIND_OPS := --leak-check=full --show-reachable=yes  --error-exitcode=1 

valgrind: dragon-parser
	rm -f ../examples/loops/.WhileLoop.dg.tokens
	dragon-lexer ../examples/loops/WhileLoop.dg
	valgrind $(VALGRIND_OPS) ./dragon-parser ../examples/loops/.WhileLoop.dg.tokens
