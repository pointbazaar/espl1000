
INCLUDE := -I..

FLAGS := -march=native -g --std=c11 -Wall -Werror $(INCLUDE)

CALL := gcc $(FLAGS) -c

OBJS := build/lvst.o \
	build/sst.o \
	build/sst_prefill.o \
	build/stst.o \
	build/symtable.o \
	build/cc.o

all: dirs $(OBJS) test

test:
	#TODO
	@echo "currently no tests for tables"

clean:
	rm -f build/*.o

dirs:
	mkdir -p build


# ---------------------------------

build/sst.o: sst/sst.c sst/sst.h
	$(CALL) sst/sst.c -o $@

build/sst_prefill.o: sst/sst_prefill.c sst/sst_prefill.h
	$(CALL) sst/sst_prefill.c -o $@

build/lvst.o: lvst/lvst.c lvst/lvst.h
	$(CALL) lvst/lvst.c -o $@

build/stst.o: stst/stst.c stst/stst.h
	$(CALL) stst/stst.c -o $@
	
build/symtable.o: symtable/symtable.c symtable/symtable.h
	$(CALL) symtable/symtable.c -o $@

build/cc.o: cc/cc.c cc/cc.h
	$(CALL) cc/cc.c -o $@
