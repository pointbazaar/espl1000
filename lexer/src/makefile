main:
	gcc -o main -O3 -march=native   -D DEBUG=false main.c tokens.c strutils.c lexer.c loop.c tokens.h strutils.h lexer.h loop.h states.h init_dfa.*

#build the lexer for debugging (includes debug statements)
main-debug: 
	gcc -g -O0 -o main-debug -D DEBUG=true  main.c tokens.c strutils.c lexer.c loop.c tokens.h strutils.h lexer.h loop.h states.h init_dfa.*

#build the tests
test: 
	gcc -g -o test       -D DEBUG=true  test.c tokens.c strutils.c lexer.c loop.c tokens.h strutils.h lexer.h loop.h states.h init_dfa.*
