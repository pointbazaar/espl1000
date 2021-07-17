
all: build

build: dirs
	cmake -S . -B build -H.
	cmake --build build --parallel 

clean:
	rm -rf build/*

dirs:
	mkdir -p build
