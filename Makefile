
all: 
	cd lexer      && make 
	cd parser     && make 
	cd transpiler && make 
	cd examples && ./build.sh

all_no_valgrind:
	cd lexer && make all_no_valgrind
	cd parser && make all_no_valgrind
	cd transpiler && make all_no_valgrind

clean:
	cd lexer && make clean
	cd parser && make clean
	cd transpiler && make clean
