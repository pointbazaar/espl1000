
INCLUDE := -I..

FLAGS := -march=native -g --std=c11 -Wall -Werror $(INCLUDE)

CALL := gcc $(FLAGS) -c 

OBJS := build/token.o build/TokenReader.o \
		build/TokenList.o build/TokenListTest.o

HEADERS := token/token.h \
	list/TokenList.h \
	reader/TokenReader.h \
	TokenKeys.h

all: dirs $(OBJS) test

dirs:
	mkdir -p build

tests: $(OBJS)
	gcc $(FLAGS) -o tests $(OBJS)

test: tests
	./tests

build/token.o: $(HEADERS) token/token.c
	$(CALL) token/token.c -o $@
	
build/TokenList.o: $(HEADERS) list/TokenList.c 
	$(CALL) list/TokenList.c -o $@

build/TokenListTest.o: $(HEADERS) test/TokenListTest.c 
		$(CALL) test/TokenListTest.c -o $@

build/TokenReader.o: $(HEADERS) reader/TokenReader.c 
	$(CALL) reader/TokenReader.c -o $@
	
clean:
	rm -f build/*.o
	rm -f tests
