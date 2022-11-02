#this build script
#tests that all examples transpile

#the tests are in the same order as a 'tree examples/' would show them

CALL2  := sd 

all: test

test: 
	@echo "[Examples] Compiling Example Programs..."
	$(CALL2) ifstatement/ifstatement.dg 
	$(CALL2) local_variables/simple.dg
	$(CALL2) loops/whileloop.dg 
	$(CALL2) loops/forloop.dg 
	$(CALL2) mathematics/fibonacci.dg 
	$(CALL2) methodCalls/methodcall.dg 
	$(CALL2) other/everything.dg 
	$(CALL2) switch/switch.dg 
	$(CALL2) typeinference/localvartypeinference.dg
	$(CALL2) array/array.dg

	@echo "[Examples] done"

clean:
	rm -f */*.tokens
	rm -f */*.out
