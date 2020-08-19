#it had -O0 before
FLAGS := -march=native -g -Wall
CALL := gcc $(FLAGS)
CALL2 := $(CALL) -c 

OBJS_MAIN := build/ASTWriter.o \
		build/ast_reader.o \
		build/Parser.o \
		build/Token.o \
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
		build/Variable.o
		
OBJS_TEST := build/AssignStmtTest.o \
		build/BasicTypeWrappedTest.o \
		build/BoolConstTest.o \
		build/CharConstTest.o \
		build/DeclArgTest.o \
		build/ExprTest.o \
		build/FloatConstTest.o \
		build/IfStmtTest.o \
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
		build/ParserTest.o 
		
OBJS := $(OBJS_MAIN) $(OBJS_TEST) build/util.o

AST_DEPS := ../ast/ast.h \
	../util/util.h \
	../ast/ASTWriter.h \
	../ast/ast_reader.h

#dependencies usually found in all AST-related parsing
#source files
MAIN_DEPS := src/main/commandline/TokenList.h \
	src/main/commandline/TokenKeys.h \
	src/main/commandline/Token.h \
	$(AST_DEPS)

all: DIRS dragon-parser test

DIRS:
	mkdir -p build/

clean: DIRS
	rm -f build/*.o

test: dragon-parser
	./dragon-parser -test

dragon-parser: $(OBJS)
	$(CALL) -o dragon-parser build/*.o
	
#generic rules

build/%.o: src/main/commandline/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
	
build/%.o: src/main/parsing/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
	
build/%.o: src/main/parsing/statements/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
	
build/%.o: src/main/parsing/typenodes/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/util.o: ../util/util.c ../util/util.h
	$(CALL2) ../util/util.c

build/%.o: ../ast/%.c $(AST_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<

build/%.o: src/test/astnodes/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
	
build/%.o: src/test/commandline/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
	
build/%.o: src/test/%.c $(MAIN_DEPS)
	@echo Compiling file: $(notdir $<)
	$(CALL2) -o $@ $<
