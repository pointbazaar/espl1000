
CALL2 := sd
CALLX86 := sd -x86

all: examples-avr examples-x86

examples-x86:
	@echo "[Examples] Compiling Example Programs (x86)"
	$(CALLX86) other/everything.dg
	$(CALLX86) typeinference/localvartypeinference.dg
	$(CALLX86) array/array.dg
	make -C externc
	./assert_examples.sh
	@echo "[Examples (x86)] done"

examples-avr:
	@echo "[Examples] Compiling Example Programs (AVR)"
	$(CALL2) ifstatement/true.dg
	$(CALL2) ifstatement/false.dg
	$(CALL2) local_variables/simple.dg
	$(CALL2) loops/whileloop/whileloop.dg
	$(CALL2) loops/forloop/forloop.dg
	$(CALL2) methodCalls/add/add_8.dg
	$(CALL2) methodCalls/add/add_16.dg
	$(CALL2) other/everything.dg
	$(CALL2) typeinference/localvartypeinference.dg
	$(CALL2) array/array.dg
	$(MAKE) -C led_blink_timer
	$(MAKE) -C led_blink_no_timer
	@echo "[Examples (AVR)] done"

clean:
	rm -f */*.tokens
	rm -f */*.out
