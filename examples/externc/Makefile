
all: compile run

run: /tmp/program
	/tmp/program

compile: caller.dg f1.c
	rm *.o
	gcc -c f1.c
	sd -x86 -nolink caller.dg
	ld -o /tmp/program f1.o caller.o
