
all: 
	cd lexer      && make 
	cd lexer_flex && make
	cd parser     && make 
	cd transpiler && make 
	cd examples   && make

all_no_valgrind:
	cd lexer && make all_no_valgrind
	cd lexer_flex && make
	cd parser && make all_no_valgrind
	cd transpiler && make all_no_valgrind
	cd examples && make

clean:
	cd lexer && make clean
	cd lexer_flex && make clean
	cd parser && make clean
	cd transpiler && make clean
