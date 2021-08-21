
TESTS := tests/test_matrix.dg \
	tests/test_polynomial.dg \
	tests/test_v2.dg \
	tests/test_collision2d.dg \
	tests/test_math.dg \
	tests/test_primes.dg \
	tests/collections/test_set.dg \
	tests/collections/test_stack.dg \
	tests/collections/test_intarray.dg \
	tests/collections/test_intbintree.dg \
	tests/collections/test_linkedlist.dg 

SOURCES :=  test.dg \
	base.dg \
	primes.dg \
	math.dg \
	polynomial.dg \
	matrix.dg \
	ppm.dg \
	v2.dg \
	collision2d.dg \
	collections/set.dg \
	collections/stack.dg \
	collections/intarray.dg \
	collections/intbintree.dg \
	collections/linkedlist.dg \
	externc/externc_libc.dg $(TESTS)

test: $(SOURCES) clean
	sd -g $(SOURCES)
	./a.out

clean:
	rm -f a.out test.c test.h
	rm -f *.c
