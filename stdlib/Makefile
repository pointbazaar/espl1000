
SOURCES :=  test.dg \
	bit.dg primes.dg math.dg \
	string.dg vector.dg 

test: $(SOURCES)
	sd -clean -no_typecheck $(SOURCES)
	./a.out

clean:
	rm -f a.out test.c
