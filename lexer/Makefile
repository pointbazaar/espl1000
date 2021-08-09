#Manual:
#https://westes.github.io/flex/manual/

all: dragon-lexer test

dragon-lexer: lex.yy.c
	cmake -S . -B build -H.
	cmake --build build
	cp build/dragon-lexer .
	
lex.yy.c:
	flex --header-file=lex.yy.h smalldragon.flex

clean:
	rm -rf build/
	rm -f dragon-lexer
	rm -f lex.yy.c
	rm -f lex.yy.h

test: dragon-lexer
	./build/dragon-lexer-tests
