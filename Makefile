
all: build-step test examples

build-step: deps build compile

build:
	cmake -S . -B build

compile:
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

setup_path:
	export PATH=${PATH}:$(pwd)/build/lexer/
	export PATH=${PATH}:$(pwd)/build/parser/
	export PATH=${PATH}:$(pwd)/build/compiler/main
	export PATH=${PATH}:$(pwd)/build/compiler/test

test: build-step ${TESTS} setup_path
	./build/tac/sd-tac-test && \
	./build/ast/sd-ast-test && \
	./build/token/sd-token-tests && \
	./build/rat/sd-rat-tests && \
	./build/lexer/dragon-lexer-tests && \
	./build/parser/dragon-parser-tests && \
	cd compiler && \
	../build/compiler/test/sd-tests && \
	cd .. && \
	cd examples   && make test
	#cd stdlib     && make test

TESTS=\
      build/tac/sd-tac-test \
      build/ast/sd-ast-test \
      build/token/sd-token-tests \
      build/rat/sd-rat-tests \
      build/lexer/dragon-lexer-tests \
      build/parser/dragon-parser-tests \
      build/compiler/test/sd-tests \

VALGRIND_OPTS=--leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

ci_valgrind_tests: ${TESTS} setup_path
	export CI_DIR=/home/circleci/project
	export BUILD_DIR=${CI_DIR}/build
	make valgrind_tests

valgrind_tests: ${TESTS}
	valgrind ${VALGRIND_OPTS} ./build/tac/sd-tac-test && \
	valgrind ${VALGRIND_OPTS} ./build/ast/sd-ast-test && \
	valgrind ${VALGRIND_OPTS} ./build/token/sd-token-tests && \
	valgrind ${VALGRIND_OPTS} ./build/rat/sd-rat-tests && \
	valgrind ${VALGRIND_OPTS} ./build/lexer/dragon-lexer-tests && \
	valgrind ${VALGRIND_OPTS} ./build/parser/dragon-parser-tests

	#TODO: enable valgrind for the compiler tests

clean_cmake:
	rm -rf CMakeFiles
	rm -rf build

clean: clean_cmake
	# we don't clean our dependencies
	cd compiler   && make clean
	cd examples   && make clean
	cd stdlib     && make clean
