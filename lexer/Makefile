#https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory

#generate profiling information with -pg flag
CALL_RELEASE := gcc -march=native -Wall -g --std=c11

#the common .o files

COMMON_RELEASE := build/release/strutils.o \
	build/release/lexer.o \
	build/release/loop.o \
	build/release/init_dfa.o \
	build/release/test.o \
	build/release/loop_io.o

all: directories dragon-lexer test valgrind

clean:
	rm -f build/release/*.o

test: dragon-lexer
	./dragon-lexer -test

VALGRIND_OPS := --leak-check=full --show-reachable=yes --error-exitcode=1 --log-file="/dev/null"

valgrind: dragon-lexer
	valgrind $(VALGRIND_OPS) ./dragon-lexer -clean ../examples/Everything.dg

directories: build/release

build/release:
	mkdir -p build/release

#the performance, release build.
#i guess we have to compile everything together here so
#it can perform optimizations for the whole program

dragon-lexer: build/release/main.o $(COMMON_RELEASE)
	$(CALL_RELEASE) -o dragon-lexer  build/release/main.o $(COMMON_RELEASE)

# the .o files, 
# generic rules

build/release/%.o: src/%.c
	@echo Compiling file: $(notdir $<)
	$(CALL_RELEASE) -c -o $@ $<

