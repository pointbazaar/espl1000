
INCLUDE := -I..

FLAGS := -march=native -g --std=c11 -Wall -Werror $(INCLUDE)

CALL := gcc $(FLAGS) -c 

OBJS := build/token.o build/token_reader.o \
		build/TokenList.o build/TokenListTest.o

HEADERS := token/token.h \
	list/TokenList.h \
	reader/token_reader.h \
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

build/token_reader.o: $(HEADERS) reader/token_reader.c 
	$(CALL) reader/token_reader.c -o $@
	
clean:
	rm -f build/*.o
	rm -f tests
