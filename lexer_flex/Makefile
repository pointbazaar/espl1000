
#Manual:
#https://westes.github.io/flex/manual/

all: smalldragon.flex driver.c driver.h
	flex smalldragon.flex
	gcc -o lexer driver.c lex.yy.c -lfl

clean:
	rm lexer
	rm lex.yy.c
