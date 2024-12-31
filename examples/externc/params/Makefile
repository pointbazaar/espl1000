
all: compile run

run: /tmp/program
	/tmp/program

compile: caller_params.dg f2.c
	rm -f *.o
	gcc -g -c f2.c
	sd -x86 -nolink caller_params.dg
	ld -o /tmp/program f2.o caller_params.o
