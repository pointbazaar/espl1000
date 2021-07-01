
all: build test

#https://cliutils.gitlab.io/modern-cmake/chapters/intro/running.html

build: DIRS
	cmake -S . -B build
	cmake --build build
	#make -C build/
	cp build/dragon-parser .

DIRS:
	mkdir -p build/

test: dragon-parser
	./dragon-parser -test

clean: DIRS
	rm -rf build/*
	rm -rf dragon-parser

VALGRIND_OPS := --leak-check=full --show-reachable=yes  --error-exitcode=1 

valgrind: dragon-parser
	rm -f ../examples/loops/.WhileLoop.dg.tokens
	rm -f ../examples/loops/.WhileLoop.dg.ast
	dragon-lexer ../examples/loops/WhileLoop.dg
	valgrind $(VALGRIND_OPS) ./dragon-parser ../examples/loops/.WhileLoop.dg.tokens
