
all: build test examples

build: deps
	cmake -S . -B build
	#cmake --build build --parallel
	cmake --build build

deps:
	cd dependencies && make

examples:
	cd examples   && make

stdlib:
	cd stdlib     && make

ci: check-format all

check-format:
	./check-format.sh

test: build
	export PATH=${PATH}:$(pwd)/build/lexer/:$(pwd)/build/parser:$(pwd)/build/compiler
	#cd token      && make test
	cd ast        && make test
	cd lexer      && make test
	cd parser     && make test
	#cd tables     && make test
	cd rat		   && make test
	cd compiler    && make test
	cd examples   && make test
	#cd stdlib     && make test

clean_cmake:
	rm -rf CMakeFiles
	rm -rf build

clean: clean_cmake
	# we don't clean our dependencies
	cd compiler   && make clean
	cd examples   && make clean
	cd stdlib     && make clean
