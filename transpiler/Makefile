FLAGS := -O0 -march=native -g -Wall

all: smalldragon

clean: 
	rm *.o

smalldragon: main
	gcc $(FLAGS) -o smalldragon *.o

main: transpiler.c
	gcc $(FLAGS) -c transpiler.c
