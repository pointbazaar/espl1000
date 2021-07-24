SOURCES = main.c main2.dg

#Some adjustment to the sd-generated main2.c file
#is required to get this example working. 
#It is a proof of concept of smalldragon C interop.

#considered for the future:
# - passthrough #include directives

#smalldragon variant
sd: main2.dg externc_sdl.dg

	sd main2.dg ../stdlib/externc/externc_sdl.dg ../../stdlib/v2.dg ../../stdlib/math.dg
	
	gcc -g main2.c -w -lSDL2 -lm

clean:
	rm -f main2.c 
