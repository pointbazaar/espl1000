
#the ast module is used as needed by other components
#of smalldragon

#to make the binary .ast format efficient, there is no use of
#redundant fields to check if the file is corrupted
#or if we are reading at the correct offset.
#so tests are needed to see that every serialization/deserialization
#works

CFLAGS := -march=native -g

CALL := gcc $(CFLAGS) -c

SOURCES := test/test_ast.c \
		io/ast_reader.c \
		io/ast_writer.c \
		io/serialize.c \
		util/free_ast.c \
		util/copy_ast.c \
		util/str_ast.c \
		visitor/vistior.c

HEADERS := io/ast_reader.h \
		io/ast_writer.h \
		util/free_ast.h \
		util/copy_ast.h \
		util/str_ast.h \
		ast.h \
		io/magic_num.h \
		io/serialize.h \
		visitor/visitor.h

OBJS := build/test_ast.o \
	build/ast_reader.o \
	build/ast_writer.o \
	build/free_ast.o \
	build/copy_ast.o \
	build/str_ast.o \
	build/serialize.o \
	build/visitor.o

all: dirs $(OBJS) tests
	./tests

tests: dirs $(OBJS)
	gcc $(CFLAGS) -o tests $(OBJS)

test: tests
	./tests

dirs:
	mkdir -p build
	
build/%.o: util/%.c $(HEADERS)
	@echo Compiling file: $(notdir $<)
	$(CALL) -o $@ $<
	

build/visitor.o: visitor/visitor.c $(HEADERS)
	$(CALL) -c visitor/visitor.c -o $@
	
	
build/%.o: io/%.c $(HEADERS)
	@echo Compiling file: $(notdir $<)
	$(CALL) -o $@ $<
	
build/%.o: test/%.c $(HEADERS)
	@echo Compiling file: $(notdir $<)
	$(CALL) -o $@ $<

clean:
	rm -f build/*.o
	rm -f tests
