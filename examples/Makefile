
CALL2 := sd
CALLX86 := sd -x86

all: examples-avr examples-x86

examples-x86:
	@echo "[Examples] Compiling Example Programs (x86)"
	$(CALLX86) ifstatement/ifstatement.dg
	$(CALLX86) local_variables/simple.dg
	$(CALLX86) loops/whileloop.dg
	$(CALLX86) loops/forloop.dg
	$(CALLX86) mathematics/fibonacci.dg
	$(CALLX86) methodCalls/methodcall.dg
	$(CALLX86) other/everything.dg
	$(CALLX86) typeinference/localvartypeinference.dg
	$(CALLX86) array/array.dg
	make -C externc
	@echo "[Examples (x86)] done"

examples-avr:
	@echo "[Examples] Compiling Example Programs (AVR)"
	$(CALL2) ifstatement/ifstatement.dg
	$(CALL2) local_variables/simple.dg
	$(CALL2) loops/whileloop.dg
	$(CALL2) loops/forloop.dg
	$(CALL2) mathematics/fibonacci.dg
	$(CALL2) methodCalls/methodcall.dg
	$(CALL2) other/everything.dg
	$(CALL2) typeinference/localvartypeinference.dg
	$(CALL2) array/array.dg
	$(MAKE) -C led_blink_timer
	$(MAKE) -C led_blink_no_timer
	@echo "[Examples (AVR)] done"

clean:
	rm -f */*.tokens
	rm -f */*.out
