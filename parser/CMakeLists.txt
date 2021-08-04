cmake_minimum_required(VERSION 3.10.2)
project(dragon-parser C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)

#use -pg to generate profiling information for gprof

add_executable(${PROJECT_NAME}
	main/commandline/main.c
	main/commandline/parser.c
	main/commandline/parser_help.c
	main/commandline/parser_flags.c
	main/commandline/parser_help.c
	
	main/util/parse_astnode.c
	
	main/astnodes/const/BinConst.c
	main/astnodes/const/BoolConst.c
	main/astnodes/const/CharConst.c
	main/astnodes/const/FloatConst.c
	main/astnodes/const/HexConst.c
	main/astnodes/const/IntConst.c
	main/astnodes/const/ConstValue.c
	main/astnodes/const/StringConst.c
	
	main/astnodes/expr/Expr.c
	main/astnodes/expr/Op.c
	main/astnodes/expr/Term.c
	main/astnodes/expr/UnOpTerm.c
	
	main/astnodes/Identifier.c
	main/astnodes/Lambda.c
	main/astnodes/Namespace.c
	main/astnodes/Range.c
	main/astnodes/StmtBlock.c
	main/astnodes/ExternC.c
	
	main/astnodes/statements/AssignStmt.c
	main/astnodes/statements/Call.c
	main/astnodes/statements/CaseStmt.c
	main/astnodes/statements/ForStmt.c
	main/astnodes/statements/IfStmt.c
        main/astnodes/statements/RetStmt.c
	main/astnodes/statements/Stmt.c
	main/astnodes/statements/SwitchStmt.c
	main/astnodes/statements/WhileStmt.c
	main/astnodes/statements/TryCatchStmt.c
	
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
	
	#token dependency
	../token/list/TokenList.c
	../token/reader/token_reader.c
	../token/token/token.c
	
	#tests
	test/astnodes/const/BoolConstTest.c
	test/astnodes/const/CharConstTest.c
	test/astnodes/const/FloatConstTest.c
	test/astnodes/expr/ExprTest.c
	test/astnodes/expr/TermTest.c
	test/astnodes/expr/UnOpTermTest.c
	test/astnodes/LambdaTest.c
	test/astnodes/NamespaceTest.c
	test/astnodes/RangeTest.c
	test/astnodes/StmtBlockTest.c
	test/astnodes/statements/AssignStmtTest.c
	test/astnodes/statements/CallTest.c
	test/astnodes/statements/CaseStmtTest.c
	test/astnodes/statements/ForStmtTest.c
	test/astnodes/statements/IfStmtTest.c
        test/astnodes/statements/RetStmtTest.c
	test/astnodes/statements/StmtTest.c
	test/astnodes/statements/SwitchStmtTest.c
	test/astnodes/statements/WhileStmtTest.c
	test/astnodes/statements/TryCatchStmtTest.c
	test/astnodes/struct/StructDeclTest.c
	test/astnodes/struct/StructMemberTest.c
	test/astnodes/subr/DeclArgTest.c
	test/astnodes/subr/MethodTest.c
	test/astnodes/types/BasicTypeTest.c
	test/astnodes/types/SimpleTypeTest.c
	test/astnodes/types/SubrTypeTest.c
	test/astnodes/var/SimpleVarTest.c
	test/astnodes/var/VariableTest.c
	test/commandline/ParserTest.c
        test/astnodes/types/StructTypeTest.c test/astnodes/types/StructTypeTest.h)

target_compile_options(${PROJECT_NAME} PUBLIC "-g" "-march=native" "-Wall" "-Wextra" "-Werror")

#AST dependency
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/../ast/build/libsd-ast.a)

#Token dependency
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/../token/build/libsd-token.a)

target_include_directories(${PROJECT_NAME} PUBLIC "main/commandline")
target_include_directories(${PROJECT_NAME} PUBLIC "main/astnodes")
target_include_directories(${PROJECT_NAME} PUBLIC "main/util")

target_include_directories(${PROJECT_NAME} PUBLIC "..")
