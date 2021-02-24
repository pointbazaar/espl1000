#Manual:
#https://westes.github.io/flex/manual/

FLAGS := -g -march=native -Wall --std=c11 -pedantic

SOURCES := smalldragon.flex \
			driver.c \
			driver.h \
			../token/token.c \
			test.c \
			test.h \
			lexer_flags.h \
			Makefile

all: all_no_valgrind valgrind

all_no_valgrind: dragon-lexer test

dragon-lexer: $(SOURCES)
	flex smalldragon.flex
	gcc $(FLAGS) -o dragon-lexer driver.c ../token/token.c test.c lex.yy.c -lfl

clean:
	rm -f dragon-lexer
	rm -f lex.yy.c

test: dragon-lexer
	./dragon-lexer -test

VALGRIND_OPS := --leak-check=full --show-reachable=yes --error-exitcode=1 --log-file="/dev/null"

valgrind: dragon-lexer
	valgrind $(VALGRIND_OPS) ./dragon-lexer -clean ../examples/other/Everything.dg
