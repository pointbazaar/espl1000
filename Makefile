
all: 
	cd lexer      && make -j
	cd parser     && make -j
	cd transpiler && make -j

all_no_valgrind:
	cd lexer && make -j all_no_valgrind
	cd parser && make -j all_no_valgrind
	cd transpiler && make -j all_no_valgrind

clean:
	cd lexer && make clean
	cd parser && make clean
	cd transpiler && make clean
