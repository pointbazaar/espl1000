
all: build test examples

build: deps
	cmake -S . -B build
	cmake --build build --parallel
	#cmake --build build

deps:
	cd dependencies && make

examples:
	cd examples   && make

stdlib:
	cd stdlib     && make

check-format:
	./check-format.sh

test: build ${TESTS}
	export PATH=${PATH}:$(pwd)/build/lexer/:$(pwd)/build/parser:$(pwd)/build/compiler
	./build/ast/sd-ast-test && \
	./build/rat/sd-rat-tests && \
	./build/lexer/dragon-lexer-tests && \
	./build/parser/dragon-parser-tests && \
	cd compiler && \
	../build/compiler/sd-tests && \
	cd .. && \
	cd examples   && make test
	#cd stdlib     && make test

TESTS=build/ast/sd-ast-test \
      build/rat/sd-rat-tests \
      build/lexer/dragon-lexer-tests \
      build/parser/dragon-parser-tests \
      build/compiler/sd-tests \

clean_cmake:
	rm -rf CMakeFiles
	rm -rf build

clean: clean_cmake
	# we don't clean our dependencies
	cd compiler   && make clean
	cd examples   && make clean
	cd stdlib     && make clean
