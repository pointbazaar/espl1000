
test: ../build/parser/dragon-parser
	./../build/parser/dragon-parser-tests

VALGRIND_OPS := --leak-check=full --show-reachable=yes  --error-exitcode=1 

valgrind: dragon-parser
	rm -f ../examples/loops/.WhileLoop.dg.tokens
	dragon-lexer ../examples/loops/WhileLoop.dg
	valgrind $(VALGRIND_OPS) ./dragon-parser ../examples/loops/.WhileLoop.dg.tokens
