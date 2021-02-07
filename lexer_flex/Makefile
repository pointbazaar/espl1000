#Manual:
#https://westes.github.io/flex/manual/

FLAGS := -march=native -Wall -Werror --std=c11 -pedantic

all: smalldragon.flex driver.c driver.h ../token/token.c
	flex smalldragon.flex
	gcc -o dragon-lexer driver.c ../token/token.c test.c lex.yy.c -lfl

clean:
	rm dragon-lexer
	rm lex.yy.c

test: lexer
	./dragon-lexer -test

VALGRIND_OPS := --leak-check=full --show-reachable=yes --error-exitcode=1 --log-file="/dev/null"

valgrind: dragon-lexer
	valgrind $(VALGRIND_OPS) ./dragon-lexer -clean ../examples/other/Everything.dg
