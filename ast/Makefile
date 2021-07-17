
all: build test
	
build: dirs
	cmake -S . -B build -H.
	cmake --build build --parallel

test: build
	mv build/sd-ast-test .
	./sd-ast-test
	mv sd-ast-test build/

dirs:
	mkdir -p build

clean:
	rm -rf build/*
	rm -f tests
