
clean:
	rm -f sd
	rm -f .*.dg .*.asm .*.hex .*.obj .*.cof

test: ../build/compiler/sd-tests
	export PATH=${PATH}:$(pwd)/build/lexer/:$(pwd)/build/parser:$(pwd)/build/compiler
	../build/compiler/sd-tests

VALGRIND_OPS := --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 

valgrind: sd
	valgrind $(VALGRIND_OPS) ./sd ../examples/other/everything.dg



