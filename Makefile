
all: 
	cd ast        && make
	cd lexer_flex && make
	cd parser     && make 
	cd transpiler && make 
	cd examples   && make

all_no_valgrind:
	cd ast        && make
	cd lexer_flex && make 
	cd parser     && make all_no_valgrind
	cd transpiler && make all_no_valgrind
	cd examples   && make

clean:
	cd ast        && make clean
	cd lexer_flex && make clean
	cd parser     && make clean
	cd transpiler && make clean
