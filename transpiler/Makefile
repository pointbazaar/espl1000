
all: build test


build: DIRS
	cmake -S . -B build
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
	valgrind $(VALGRIND_OPS) ./sd -clean ../examples/other/Everything.dg



