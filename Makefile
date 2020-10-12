
#this Makefile is best run with sudo.
#don't ask me why

all: 
	cd lexer      && make -j
	cd parser     && make -j
	cd transpiler && make -j

clean:
	cd lexer && make clean
	cd parser && make clean
	cd transpiler && make clean
