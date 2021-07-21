
SOURCES :=  test.dg \
	primes.dg math.dg \
	intset.dg intstack.dg intarray.dg intbintree.dg intll.dg \
	complex.dg \
	polynomial.dg \
	matrix.dg \
	ppm.dg \
	v2.dg \
	collision2d.dg

test: $(SOURCES) clean
	sd -g $(SOURCES)
	./a.out

clean:
	rm -f a.out test.c test.h
	rm -f *.c
