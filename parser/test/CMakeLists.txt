
add_library(sd-parser-test

	#tests
	astnodes/const/ConstValueTest.c

	astnodes/expr/ExprTest.c
	astnodes/expr/TermTest.c
	astnodes/expr/UnOpTermTest.c
	astnodes/expr/DerefTest.c
	astnodes/expr/AddressOfTest.c

	astnodes/NamespaceTest.c

	astnodes/RangeTest.c

	astnodes/StmtBlockTest.c

	astnodes/statements/AssignStmtTest.c
	astnodes/statements/CallTest.c
	astnodes/statements/ForStmtTest.c
	astnodes/statements/IfStmtTest.c
	astnodes/statements/RetStmtTest.c
	astnodes/statements/StmtTest.c
	astnodes/statements/WhileStmtTest.c

	astnodes/struct/StructDeclTest.c
	astnodes/struct/StructMemberTest.c

	astnodes/subr/DeclArgTest.c
	astnodes/subr/MethodTest.c

	astnodes/types/ParseTypeTests.h
	astnodes/types/BasicTypeTest.c
	astnodes/types/SimpleTypeTest.c
	astnodes/types/SubrTypeTest.c
	astnodes/types/StructTypeTest.c
	astnodes/types/PointerTypeTest.c

	astnodes/var/SimpleVarTest.c
	astnodes/var/VariableTest.c

	commandline/ParserTest.c

	test_parser_util.c
	test_parser_util.h
)

target_link_libraries(sd-parser-test dragon-parser-base sd-util sd-ast sd-token)

target_include_directories(sd-parser-test PUBLIC "main/commandline")
target_include_directories(sd-parser-test PUBLIC "main/astnodes")
target_include_directories(sd-parser-test PUBLIC "main/util")
target_include_directories(sd-parser-test PUBLIC "..")


