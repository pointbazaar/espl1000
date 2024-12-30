
all: build-step test compile-examples

build-step: deps build compile

build:
	cmake -S . -B build

compile:
	cmake --build build --parallel
	#cmake --build build

deps:
	cd dependencies && make

compile-examples:
	make -C examples

stdlib:
	cd stdlib     && make

check-format:
	./check-format.sh

setup_path:
	export PATH=${PATH}:$(pwd)/build/compiler/main
	export PATH=${PATH}:$(pwd)/build/compiler/test

TESTBIN=build/compiler/test/sd-tests

${TESTBIN}: build-step

test: ${TESTBIN} setup_path
	./build/compiler/test/sd-tests
	#cd stdlib     && make test

VALGRIND_OPTS=--leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1

VALGRIND_CALL=${VALGRIND_OPTS} ${TESTBIN}

valgrind_tests: ${TESTBIN} setup_path
	# following 2 steps for circle ci
	export CI_DIR=/home/circleci/project
	export BUILD_DIR=${CI_DIR}/build
	valgrind ${VALGRIND_CALL} -testsuite Token
	valgrind ${VALGRIND_CALL} -testsuite Lexer
	valgrind ${VALGRIND_CALL} -testsuite AST
	valgrind ${VALGRIND_CALL} -testsuite Parser
	valgrind ${VALGRIND_CALL} -testsuite RAT
	valgrind ${VALGRIND_CALL} -testsuite TAC
	valgrind ${VALGRIND_CALL} -testsuite Typeinference
	#valgrind ${VALGRIND_CALL} -testsuite Typechecker
	#valgrind ${VALGRIND_CALL} -testsuite "AVR CodeGen"
	#valgrind ${VALGRIND_CALL} -testsuite "TAC CodeGen from Snippets"
	#valgrind ${VALGRIND_CALL} -testsuite "x86 CodeGen"
	valgrind ${VALGRIND_CALL} -testsuite "Liveness"
	valgrind ${VALGRIND_CALL} -testsuite "Tables"

clean_cmake:
	rm -rf CMakeFiles
	rm -rf build

clean: clean_cmake
	# we don't clean our dependencies
	cd compiler   && make clean
	cd examples   && make clean
	cd stdlib     && make clean
