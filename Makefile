
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
	export PATH=${PATH}:$(pwd)/build/compiler/main
	export PATH=${PATH}:$(pwd)/build/compiler/test

test: build-step ${TESTS} setup_path
	cd compiler && \
	../build/compiler/test/sd-tests && \
	cd .. && \
	cd examples   && make test
	#cd stdlib     && make test

TESTS=\
      build/compiler/test/sd-tests \

VALGRIND_OPTS=--leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

ci_valgrind_tests: ${TESTS} setup_path
	export CI_DIR=/home/circleci/project
	export BUILD_DIR=${CI_DIR}/build
	make valgrind_tests

valgrind_tests: ${TESTS}
	#valgrind ${VALGRIND_OPTS} ./build/parser/dragon-parser-tests
	echo "enable valgrind tests for sd-tests"
	#TODO: enable valgrind for the compiler tests

clean_cmake:
	rm -rf CMakeFiles
	rm -rf build

clean: clean_cmake
	# we don't clean our dependencies
	cd compiler   && make clean
	cd examples   && make clean
	cd stdlib     && make clean
