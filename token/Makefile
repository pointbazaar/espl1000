
FLAGS := -march=native -g --std=c11 -Wall -Werror

CALL := gcc $(FLAGS) -c 

OBJS := build/token.o build/TokenReader.o \
		build/TokenList.o build/TokenListTest.o

HEADERS := token.h TokenList.h TokenReader.h TokenKeys.h

all: dirs $(OBJS) test

dirs:
	mkdir -p build

test: $(OBJS)
	gcc $(FLAGS) -o test $(OBJS)
	./test

build/token.o: $(HEADERS) token.c
	$(CALL) token.c -o $@
	
build/TokenList.o: $(HEADERS) TokenList.c 
	$(CALL) TokenList.c -o $@

build/TokenListTest.o: $(HEADERS) TokenListTest.c 
		$(CALL) TokenListTest.c -o $@

build/TokenReader.o: $(HEADERS) TokenReader.c 
	$(CALL) TokenReader.c -o $@
	
clean:
	rm -f build/*.o
	rm test
