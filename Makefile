
all:
	cd dependencies && make
	cd token      && make
	cd ast        && make
	cd lexer      && make
	cd parser     && make 
	cd tables     && make
	cd rat        && make
	cd tac        && make
	cd compiler    && make 
	#cd examples   && make
	#cd stdlib     && make

test:
	# we don't test our dependencies
	#cd token      && make test
	cd ast        && make test
	cd lexer      && make test
	cd parser     && make test
	#cd tables     && make test
	cd compiler    && make test
	#cd examples   && make test
	#cd stdlib     && make test

clean:
	# we don't clean our dependencies
	cd token      && make clean
	cd ast        && make clean
	cd lexer      && make clean
	cd parser     && make clean
	cd tables     && make clean
	cd rat        && make clean
	cd tac        && make clean
	cd compiler   && make clean
	cd examples   && make clean
	cd stdlib     && make clean
