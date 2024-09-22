
TESTS := tests/test_matrix.dg \
	#tests/test_math.dg \
	#tests/collections/test_intbintree.dg \
	#tests/collections/test_arraylist.dg \
	#tests/collections/test_linkedlist.dg \
	#tests/collections/test_array.dg \
	#tests/test_primes.dg \
	#tests/test_polynomial.dg \
	#tests/collections/test_set.dg \
	#tests/collections/test_stack.dg \

SOURCES :=  test.dg \
	collections/linkedlist.dg \
	collections/arraylist.dg \
	collections/array.dg \
	collections/set.dg \
	collections/stack.dg \
	collections/intbintree.dg \
	base/math.dg \
	base/primes.dg \
	base/polynomial.dg \
	avr/portb.dg \
	avr/timer0.dg \

test: $(SOURCES) clean
	sd $(SOURCES)

debug: $(SOURCES) clean
	sd -debug $(SOURCES)

clean:
	rm -f a.out
	rm -f *.c *.asm *.hex *.obj
	rm -f *.dot *.png
