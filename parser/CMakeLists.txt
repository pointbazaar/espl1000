
#use -pg to generate profiling information for gprof

add_library(dragon-parser-base

	main/util/parser.c
	main/util/parse_astnode.c

	main/astnodes/const/IntConst.c
	main/astnodes/const/ConstValue.c
	main/astnodes/const/StringConst.c

	main/astnodes/expr/Expr.c
	main/astnodes/expr/Op.c
	main/astnodes/expr/Term.c
	main/astnodes/expr/UnOpTerm.c
	main/astnodes/expr/MDirect.c

	main/astnodes/Identifier.c
	main/astnodes/Namespace.c
	main/astnodes/Range.c
	main/astnodes/StmtBlock.c

	main/astnodes/statements/AssignStmt.c
	main/astnodes/statements/MAssignStmt.c
	main/astnodes/statements/Call.c
	main/astnodes/statements/ForStmt.c
	main/astnodes/statements/IfStmt.c
	main/astnodes/statements/RetStmt.c
	main/astnodes/statements/Stmt.c
	main/astnodes/statements/WhileStmt.c

	main/astnodes/struct/StructDecl.c
	main/astnodes/struct/StructMember.c

	main/astnodes/subr/DeclArg.c
	main/astnodes/subr/MethodDecl.c
	main/astnodes/subr/Method.c

	main/astnodes/types/ArrayType.c
	main/astnodes/types/BasicType.c
	main/astnodes/types/PrimitiveType.c
	main/astnodes/types/SimpleType.c
	main/astnodes/types/StructType.c
	main/astnodes/types/SubrType.c
	main/astnodes/types/Type.c
	main/astnodes/types/TypeParam.c

	main/astnodes/var/SimpleVar.c
	main/astnodes/var/Variable.c
)

add_executable(dragon-parser-tests

	#tests
	test/astnodes/const/ConstValueTest.c
	
	test/astnodes/expr/ExprTest.c
	test/astnodes/expr/TermTest.c
	test/astnodes/expr/UnOpTermTest.c
	test/astnodes/expr/MDirectTest.c
	
	test/astnodes/NamespaceTest.c
	
	test/astnodes/RangeTest.c
	
	test/astnodes/StmtBlockTest.c
	
	test/astnodes/statements/AssignStmtTest.c
	test/astnodes/statements/MAssignStmtTest.c
	test/astnodes/statements/CallTest.c
	test/astnodes/statements/ForStmtTest.c
	test/astnodes/statements/IfStmtTest.c
	test/astnodes/statements/RetStmtTest.c
	test/astnodes/statements/StmtTest.c
	test/astnodes/statements/WhileStmtTest.c

    test/astnodes/struct/StructDeclTest.c
	test/astnodes/struct/StructMemberTest.c
	
	test/astnodes/subr/DeclArgTest.c
	test/astnodes/subr/MethodTest.c
	
	test/astnodes/types/BasicTypeTest.c
	test/astnodes/types/BasicTypeTest.h
	test/astnodes/types/SimpleTypeTest.c
	test/astnodes/types/SimpleTypeTest.h
	test/astnodes/types/SubrTypeTest.c
	test/astnodes/types/SubrTypeTest.h
	test/astnodes/types/StructTypeTest.c
	test/astnodes/types/StructTypeTest.h
	
	test/astnodes/var/SimpleVarTest.c
	test/astnodes/var/VariableTest.c
	
	test/commandline/ParserTest.c
	
	test/test_parser_util.c 
	test/test_parser_util.h
)

add_executable(dragon-parser
	main/commandline/main.c
	main/commandline/parser_help.c
	main/commandline/parser_flags.c
	main/commandline/parser_help.c
)

target_compile_options(dragon-parser PUBLIC ${COMMON_OPTS})
target_compile_options(dragon-parser-base PUBLIC ${COMMON_OPTS})
target_compile_options(dragon-parser-tests PUBLIC ${COMMON_OPTS})

target_link_libraries(dragon-parser-base sd-util sd-ast sd-token)

target_link_libraries(dragon-parser dragon-parser-base sd-util sd-ast sd-token)

target_link_libraries(dragon-parser-tests dragon-parser-base sd-util sd-ast sd-token)

target_include_directories(dragon-parser-base PUBLIC "main/commandline")
target_include_directories(dragon-parser-base PUBLIC "main/astnodes")
target_include_directories(dragon-parser-base PUBLIC "main/util")
target_include_directories(dragon-parser-base PUBLIC "..")

target_include_directories(dragon-parser PUBLIC "main/commandline")
target_include_directories(dragon-parser PUBLIC "main/astnodes")
target_include_directories(dragon-parser PUBLIC "main/util")
target_include_directories(dragon-parser PUBLIC "..")

target_include_directories(dragon-parser-tests PUBLIC "main/astnodes")
target_include_directories(dragon-parser-tests PUBLIC "main/util")
target_include_directories(dragon-parser-tests PUBLIC "..")
