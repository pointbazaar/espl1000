
TESTS := tests/test_intbintree.dg \
	tests/test_matrix.dg \
	tests/test_polynomial.dg \
	tests/test_set.dg \
	tests/test_stack.dg \
	tests/test_v2.dg \
	tests/test_collision2d.dg \
	tests/test_math.dg \
	tests/test_primes.dg \
	tests/test_linkedlist.dg \
	tests/test_intarray.dg

SOURCES :=  test.dg \
	base.dg \
	primes.dg \
	math.dg \
	set.dg \
	stack.dg \
	intarray.dg \
	intbintree.dg \
	linkedlist.dg \
	polynomial.dg \
	matrix.dg \
	ppm.dg \
	v2.dg \
	collision2d.dg \
	externc/externc_libc.dg $(TESTS)

test: $(SOURCES) clean
	sd -g $(SOURCES)
	./a.out

clean:
	rm -f a.out test.c test.h
	rm -f *.c
