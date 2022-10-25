all: build test

build: DIRS
	cmake -S . -B build -H.
	cmake --build build --parallel
	cp build/sd .

clean: 
	rm -rf build/*
	rm -f sd
	rm -f .*.dg .*.asm .*.hex .*.obj .*.cof

test: build/sd-tests
	./build/sd-tests
	
DIRS:
	mkdir -p build

VALGRIND_OPS := --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 

valgrind: sd
	valgrind $(VALGRIND_OPS) ./sd ../examples/other/Everything.dg



