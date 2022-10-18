#this build script
#tests that all examples transpile

#the tests are in the same order as a 'tree examples/' would show them

CALL  := sd -Werror 
CALL2  := sd ../stdlib/base.dg

all: test

test: 
	@echo "[Examples] Transpiling Example Programs..."
	#$(CALL) chars/chars.dg
	#$(CALL2) functional/filter.dg 
	#$(CALL2) functional/twice.dg
	#$(CALL2) ifstatement/ifstatement.dg 
	#$(CALL2) local_variables/locals.dg 
	#$(CALL2) local_variables/locals2.dg 
	#$(CALL2) loops/whileloop.dg 
	#$(CALL2) loops/forloop.dg 
	$(CALL2) mathematics/fibonacci.dg 
	$(CALL2) methodCalls/methodcall.dg 
	#$(CALL2) other/everything.dg 
	#$(CALL2) other/harshad.dg 
	$(CALL2) structs/mystructs.dg 
	$(CALL2) structs/node.dg 
	$(CALL2) switch/switch.dg 
	$(CALL2) plotting/plotting.dg
	$(CALL2) lambda.dg
	$(CALL2) annotations.dg ../stdlib/collections/stack.dg ../stdlib/ppm.dg ../stdlib/math.dg
	$(CALL2) typeinference/localvartypeinference.dg


	$(CALL2) image_io.dg ../stdlib/collections/stack.dg ../stdlib/ppm.dg ../stdlib/math.dg
	# $(CALL2) fnptrstruct.dg #causing problems
	
	$(CALL2) generics/genericlist.dg
	$(CALL2) generics/genericstruct.dg

	@echo "[Examples] done"

# currently not working: 
# - generics/genericstruct.dg

clean:
	rm -f */*.tokens
	rm -f */*.c
	rm -f */*.out
