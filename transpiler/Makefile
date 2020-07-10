FLAGS := -O0 -march=native -g -Wall

all: smalldragon

clean: 
	rm *.o

smalldragon: main
	gcc $(FLAGS) -o sdg *.o

main: *.c ../ast/ast_reader.c
	gcc $(FLAGS) -c *.c ../ast/ast_reader.c
