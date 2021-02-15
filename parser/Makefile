#it had -O0 before
#use -pg to generate profiling information for gprof
FLAGS := -march=native -g -Wall -Werror --std=gnu11 -pedantic

CALL := gcc $(FLAGS)
CALL2 := $(CALL) -c 

OBJS_UTIL := build/ASTWriter.o \
	build/free_ast.o \
	build/copy_ast.o

OBJS_MAIN := build/Parser.o \
		build/token.o \
		build/TokenList.o \
		build/TokenReader.o \
		build/AST_Whole_Program.o \
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
		build/MethodCall.o \
		build/RetStmt.o \
		build/Stmt.o \
		build/WhileStmt.o \
		build/LoopStmt.o \
		build/BreakStmt.o \
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
		build/Variable.o \
		build/UnOpTerm.o \
		build/Range.o \
		build/ForStmt.o \
		build/SwitchStmt.o \
		build/CaseStmt.o 
		
OBJS_TEST := build/AssignStmtTest.o \
		build/BasicTypeWrappedTest.o \
		build/BoolConstTest.o \
		build/CharConstTest.o \
		build/DeclArgTest.o \
		build/ExprTest.o \
		build/FloatConstTest.o \
		build/IfStmtTest.o \
		build/LoopStmtTest.o \
		build/BreakStmtTest.o \
		build/MethodCallTest.o \
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
		build/TokenListTest.o \
		build/ParserTest.o \
		build/UnOpTermTest.o \
		build/RangeTest.o \
		build/ForStmtTest.o \
		build/SwitchStmtTest.o \
	       	build/CaseStmtTest.o	
		
OBJS := $(OBJS_MAIN) $(OBJS_TEST) $(OBJS_UTIL) 

AST_DEPS := ../ast/ast.h \
	../ast/ASTWriter.h \
	../ast/free_ast.h \
	../ast/copy_ast.h

#dependencies usually found in all AST-related parsing
#source files
MAIN_DEPS := main/commandline/TokenList.h \
	main/commandline/TokenKeys.h \
	../token/token.h \
	$(AST_DEPS)

all: all_no_valgrind valgrind

all_no_valgrind: DIRS dragon-parser test

DIRS:
	mkdir -p build/

clean: DIRS
	rm -f build/*.o

test: dragon-parser
	./dragon-parser -test

VALGRIND_OPS := --leak-check=full --show-reachable=yes  --error-exitcode=1 --log-file="/dev/null"

valgrind: dragon-parser
	rm -f ../examples/loops/.WhileLoop.dg.tokens
	rm -f ../examples/loops/.WhileLoop.dg.ast
	dragon-lexer ../examples/loops/WhileLoop.dg
	valgrind $(VALGRIND_OPS) ./dragon-parser ../examples/loops/.WhileLoop.dg.tokens

dragon-parser: $(OBJS)
	$(CALL) -o dragon-parser build/*.o
	
#generic rules

build/%.o: main/commandline/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
	
build/%.o: main/parsing/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
	
build/%.o: main/parsing/statements/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
	
build/%.o: main/parsing/typenodes/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/util.o: ../util/util.c ../util/util.h
	$(CALL2)  -o $@ ../util/util.c

build/%.o: ../ast/%.c $(AST_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/%.o: ../token/%.c ../token/%.h
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
