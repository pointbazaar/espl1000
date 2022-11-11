
all: build test

build: dirs
	cmake -S . -B build -H.
	cmake --build build --parallel 
	
test: build
	./build/sd-rat-tests

clean:
	rm -rf build/*

dirs:
	mkdir -p build
