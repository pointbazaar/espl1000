FLAGS := -O0 -march=native -g -Wall

all: smalldragon

clean: 
	rm *.o

smalldragon: main
	gcc $(FLAGS) -o smalldragon *.o

main: transpiler.c ../ast/ast_reader.c
	gcc $(FLAGS) -c transpiler.c ../ast/ast_reader.c
