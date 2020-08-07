
#this Makefile is best run with sudo.
#don't ask me why

all: 
	cd lexer && $(MAKE)
	cd parser && $(MAKE)
	cd transpiler && $(MAKE)

