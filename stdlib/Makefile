
SOURCES :=  test.dg \
	bit.dg primes.dg math.dg \
	string.dg \
	intset.dg intstack.dg intarray.dg intbintree.dg

test: $(SOURCES)
	sd -clean -no_typecheck -g $(SOURCES)
	./a.out

clean:
	rm -f a.out test.c
