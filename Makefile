
all: 
	cd token      && make
	cd ast        && make
	cd lexer      && make
	cd parser     && make 
	cd transpiler && make 
	cd examples   && make

all_no_valgrind:
	cd token      && make
	cd ast        && make
	cd lexer      && make 
	cd parser     && make all_no_valgrind
	cd transpiler && make all_no_valgrind
	cd examples   && make

clean:
	cd token      && make clean
	cd ast        && make clean
	cd lexer      && make clean
	cd parser     && make clean
	cd transpiler && make clean
	cd examples   && make clean
