#it had -O0 before
#use -pg to generate profiling information for gprof
FLAGS := -march=native -g --std=gnu11 -Wall -Wextra -Werror 

INCLUDE := -Imain/astnodes -I..

CALL := gcc $(FLAGS) $(INCLUDE)
CALL2 := $(CALL) -c 

AST_BUILD := ../ast/build

OBJS_AST := $(AST_BUILD)/ast_writer.o \
	$(AST_BUILD)/free_ast.o \
	$(AST_BUILD)/copy_ast.o \
	$(AST_BUILD)/serialize.o

TOKEN_BUILD := ../token/build

OBJS_TOKEN := $(TOKEN_BUILD)/token.o \
		$(TOKEN_BUILD)/TokenList.o \
		$(TOKEN_BUILD)/token_reader.o

OBJS_MAIN := build/parser.o \
		build/parser_help.o \
		build/parser_flags.o \
		build/BoolConst.o \
		build/CharConst.o \
		build/DeclArg.o \
		build/Expr.o \
		build/FloatConst.o \
		build/Identifier.o \
		build/IntConst.o \
		build/HexConst.o \
		build/BinConst.o \
		build/Method.o \
		build/Namespace.o \
		build/Op.o \
		build/SimpleVar.o \
		build/AssignStmt.o \
		build/IfStmt.o \
		build/Call.o \
		build/RetStmt.o \
		build/Stmt.o \
		build/WhileStmt.o \
		build/LoopStmt.o \
		build/StmtBlock.o \
		build/StringConst.o \
		build/StructDecl.o \
		build/StructMember.o \
		build/Term.o \
		build/ArrayType.o \
		build/BasicTypeWrapped.o \
		build/SimpleType.o \
		build/SubrType.o \
		build/Type.o \
		build/TypeParam.o \
		build/StructType.o \
		build/PrimitiveType.o \
		build/Variable.o \
		build/UnOpTerm.o \
		build/Range.o \
		build/ForStmt.o \
		build/SwitchStmt.o \
		build/CaseStmt.o \
		build/Lambda.o \
		build/parse_astnode.o
		
OBJS_TEST := build/AssignStmtTest.o \
		build/BasicTypeWrappedTest.o \
		build/BoolConstTest.o \
		build/CharConstTest.o \
		build/DeclArgTest.o \
		build/ExprTest.o \
		build/FloatConstTest.o \
		build/IfStmtTest.o \
		build/LoopStmtTest.o \
		build/CallTest.o \
		build/MethodTest.o \
		build/NamespaceTest.o \
		build/RetStmtTest.o \
		build/SimpleTypeTest.o \
		build/SimpleVarTest.o \
		build/StmtBlockTest.o \
		build/StmtTest.o \
		build/StructDeclTest.o \
		build/StructMemberTest.o \
		build/SubrTypeTest.o \
		build/TermTest.o \
		build/VariableTest.o \
		build/WhileStmtTest.o \
		build/ParserTest.o \
		build/UnOpTermTest.o \
		build/RangeTest.o \
		build/ForStmtTest.o \
		build/SwitchStmtTest.o \
	    build/CaseStmtTest.o \
	    build/LambdaTest.o
		
OBJS := $(OBJS_MAIN) $(OBJS_TEST) $(OBJS_AST) $(OBJS_TOKEN)

AST_DEPS := ../ast/ast.h \
	../ast/io/ast_writer.h \
	../ast/util/free_ast.h \
	../ast/util/copy_ast.h


TOKEN_DEPS := ../token/list/TokenList.h \
	../token/TokenKeys.h \
	../token/token/token.h 

MAIN_DEPS := $(AST_DEPS) $(TOKEN_DEPS)

all: DIRS dragon-parser test valgrind

DIRS:
	mkdir -p build/

clean: DIRS
	rm -f build/*.o
	rm -rf dragon-parser

test: dragon-parser
	./dragon-parser -test

VALGRIND_OPS := --leak-check=full --show-reachable=yes  --error-exitcode=1 
#--log-file="/dev/null"

valgrind: dragon-parser
	rm -f ../examples/loops/.WhileLoop.dg.tokens
	rm -f ../examples/loops/.WhileLoop.dg.ast
	dragon-lexer ../examples/loops/WhileLoop.dg
	valgrind $(VALGRIND_OPS) ./dragon-parser ../examples/loops/.WhileLoop.dg.tokens

dragon-parser: $(OBJS)
	$(CALL) -o dragon-parser $(OBJS)
	
#generic rules

build/%.o: main/commandline/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
	
build/%.o: main/util/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
	
build/%.o: main/astnodes/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
	
build/%.o: main/astnodes/var/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/%.o: main/astnodes/statements/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
	
build/%.o: main/astnodes/const/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/%.o: main/astnodes/struct/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/%.o: main/astnodes/types/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/%.o: main/astnodes/subr/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/%.o: main/astnodes/expr/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

## ------------- TEST RULES ------------- ##


build/%.o: test/astnodes/struct/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/%.o: test/astnodes/const/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/%.o: test/astnodes/statements/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/%.o: test/astnodes/var/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/%.o: test/astnodes/types/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/%.o: test/astnodes/subr/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/%.o: test/astnodes/expr/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/%.o: test/astnodes/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
	
build/%.o: test/commandline/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
	
build/%.o: test/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
