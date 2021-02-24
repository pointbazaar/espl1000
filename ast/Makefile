
#the ast module is used as needed by other components
#of smalldragon

#to make the binary .ast format efficient, there is no use of
#redundant fields to check if the file is corrupted
#or if we are reading at the correct offset.
#so tests are needed to see that every serialization/deserialization
#works

CFLAGS := -march=native -g

SOURCES := ast-tests.c \
		ast_reader.c \
		ast_writer.c \
		free_ast.c \
		copy_ast.c \
		serialize.c

HEADERS := ast_reader.h \
		ast_writer.h \
		free_ast.h \
		ast.h \
		copy_ast.h \
		magic_num.h \
		serialize.h

all: $(SOURCES) $(HEADERS)
	gcc $(CFLAGS) -o tests $(SOURCES)
	./tests

clean:
	rm -f *.o
	rm -f tests
