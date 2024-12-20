
#use -pg to generate profiling information for gprof

add_library(dragon-parser-base

	util/parser.c
	util/parse_astnode.c

	astnodes/const/IntConst.c
	astnodes/const/ConstValue.c
	astnodes/const/StringConst.c

	astnodes/expr/Expr.c
	astnodes/expr/Op.c
	astnodes/expr/Term.c
	astnodes/expr/UnOpTerm.c
	astnodes/expr/MDirect.c

	astnodes/Identifier.c
	astnodes/Namespace.c
	astnodes/Range.c
	astnodes/StmtBlock.c

	astnodes/statements/AssignStmt.c
	astnodes/statements/MAssignStmt.c
	astnodes/statements/Call.c
	astnodes/statements/ForStmt.c
	astnodes/statements/IfStmt.c
	astnodes/statements/RetStmt.c
	astnodes/statements/Stmt.c
	astnodes/statements/WhileStmt.c

	astnodes/struct/StructDecl.c
	astnodes/struct/StructMember.c

	astnodes/subr/DeclArg.c
	astnodes/subr/MethodDecl.c
	astnodes/subr/Method.c

	astnodes/types/ArrayType.c
	astnodes/types/BasicType.c
	astnodes/types/PrimitiveType.c
	astnodes/types/SimpleType.c
	astnodes/types/StructType.c
	astnodes/types/SubrType.c
	astnodes/types/Type.c
	astnodes/types/TypeParam.c

	astnodes/var/SimpleVar.c
	astnodes/var/Variable.c
)

target_compile_options(dragon-parser-base PUBLIC ${COMMON_OPTS})

target_link_libraries(dragon-parser-base sd-util sd-ast sd-token)

target_include_directories(dragon-parser-base PUBLIC "astnodes")
target_include_directories(dragon-parser-base PUBLIC "util")
target_include_directories(dragon-parser-base PUBLIC "../..")

