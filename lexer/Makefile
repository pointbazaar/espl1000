#https://stackoverflow.com/questions/5178125/how-to-place-object-files-in-separate-subdirectory

CALL_COMMON := gcc -march=native -Wall

#the gcc call for debugging and test builds
CALL_DEBUG :=         $(CALL_COMMON) -O0 -D DEBUG=true -g 

CALL_RELEASE := $(CALL_COMMON) -O3 -D DEBUG=false 

#the common .o files
COMMON_DEBUG := build/debug/strutils.o build/debug/lexer.o build/debug/loop.o build/debug/init_dfa.o 

COMMON_RELEASE := build/release/strutils.o build/release/lexer.o build/release/loop.o build/release/init_dfa.o 

all: directories dragon-lexer dragon-lexer-debug test 

directories: build/release build/debug

build/release:
	mkdir -p build/release

build/debug:
	mkdir -p build/debug

#the performance, release build.
#i guess we have to compile everything together here so
#it can perform optimizations for the whole program
dragon-lexer: build/release/main.o $(COMMON_RELEASE)
	$(CALL_RELEASE) -o dragon-lexer  build/release/main.o $(COMMON_RELEASE)

#build the lexer for debugging (includes debug statements)
dragon-lexer-debug: build/debug/main.o $(COMMON_DEBUG)
	$(CALL_DEBUG) -o dragon-lexer-debug build/debug/main.o $(COMMON_DEBUG)

#build the tests
test: build/debug/test.o $(COMMON_DEBUG)
	$(CALL_DEBUG) -o test build/debug/test.o $(COMMON_DEBUG)

# the .o files



#generic

build/debug/%.o: src/%.c
	@echo Compiling file: $(notdir $<)
	$(CALL_DEBUG) -c -o $@ $<

build/release/%.o: src/%.c
	@echo Compiling file: $(notdir $<)
	$(CALL_RELEASE) -c -o $@ $<

