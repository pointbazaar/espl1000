all: build
	
build: dirs
	cmake -S . -B build -H.
	cmake --build build --parallel

dirs:
	mkdir -p build

clean:
	rm -rf build/*
