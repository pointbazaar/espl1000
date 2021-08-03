#this build script
#tests that all examples transpile

#the tests are in the same order as a 'tree examples/' would show them

CALL  := sd -Werror ../stdlib/externc/externc_libc.dg

all: test

test: 
	@echo "[Examples] Transpiling Example Programs..."
	$(CALL) booleans/booleans.dg
	$(CALL) break/break.dg
	$(CALL) chars/chars.dg
	$(CALL) cmd-args/cmdargs.dg
	$(CALL) floats/floats.dg
	$(CALL) functional/filter.dg 
	$(CALL) functional/twice.dg
	$(CALL) ifstatement/ifstatement.dg 
	$(CALL) input/input.dg 
	$(CALL) local_variables/locals.dg 
	$(CALL) local_variables/locals2.dg 
	$(CALL) loops/whileloop.dg 
	$(CALL) loops/forloop.dg 
	$(CALL) loops/loop.dg 
	$(CALL) mathematics/fibonacci.dg 
	$(CALL) methodCalls/methodcall.dg 
	$(CALL) other/everything.dg 
	$(CALL) other/harshad.dg 
	$(CALL) structs/mystructs.dg 
	$(CALL) structs/node.dg 
	$(CALL) switch/switch.dg 
	$(CALL) file_io/file_io.dg
	$(CALL) plotting/plotting.dg
	$(CALL) lambda.dg
	$(CALL) annotations.dg ../stdlib/intstack.dg ../stdlib/ppm.dg ../stdlib/math.dg
	$(CALL) typeinference/localvartypeinference.dg

	$(CALL) try.dg
	$(CALL) try2.dg

	$(CALL) image_io.dg ../stdlib/intstack.dg ../stdlib/ppm.dg ../stdlib/math.dg
	# $(CALL) fnptrstruct.dg #causing problems
	@echo "[Examples] done"

# currently not working: 
# - generics/genericstruct.dg

clean:
	rm -f */*.tokens
	rm -f */*.ast
	rm -f */*.c
	rm -f */*.out
