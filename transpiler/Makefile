
all: build test

#older versions of cmake sadly do not support --parallel option
#that's why it's missing here

build: DIRS
	cmake -S . -B build -H.
	cmake --build build --parallel
	cp build/sd .

clean: 
	rm -rf build/*
	rm -f sd

test: sd
	./sd -test
	
DIRS:
	mkdir -p build

VALGRIND_OPS := --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 

valgrind: sd
	valgrind $(VALGRIND_OPS) ./sd ../examples/other/Everything.dg



