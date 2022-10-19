
TESTS := tests/test_matrix.dg \
	tests/test_polynomial.dg \
	tests/test_math.dg \
	tests/test_primes.dg \
	tests/collections/test_set.dg \
	tests/collections/test_stack.dg \
	tests/collections/test_intbintree.dg \
	tests/collections/test_arraylist.dg \
	tests/collections/test_linkedlist.dg 
	#tests/collections/test_array.dg \

SOURCES :=  test.dg \
	primes.dg \
	math.dg \
	polynomial.dg \
	collections/set.dg \
	collections/stack.dg \
	collections/intbintree.dg \
	collections/linkedlist.dg \
	collections/arraylist.dg 
	#collections/array.dg \

test: $(SOURCES) clean
	sd $(SOURCES) 
	./a.out

clean:
	rm -f a.out test.c test.h
	rm -f *.c
