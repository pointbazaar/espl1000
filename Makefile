
all: 
	cd token      && make
	cd ast        && make
	cd lexer      && make
	cd parser     && make 
	cd tables     && make
	cd transpiler && make 
	cd examples   && make

test:
	cd token      && make test
	cd ast        && make test
	cd lexer      && make test
	cd parser     && make test
	cd tables     && make test
	cd transpiler && make test
	cd examples   && make test

clean:
	cd token      && make clean
	cd ast        && make clean
	cd lexer      && make clean
	cd parser     && make clean
	cd tables     && make clean
	cd transpiler && make clean
	cd examples   && make clean
