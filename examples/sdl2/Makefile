SOURCES = main.c main2.dg

#Some adjustment to the sd-generated main2.c file
#is required to get this example working. 
#It is a proof of concept of smalldragon C interop.

#considered for the future:
# - passthrough #include directives

#smalldragon variant
sd: main2.dg externc_sdl.dg
	sd main2.dg externc_sdl.dg ../../stdlib/v2.dg ../../stdlib/math.dg
	
	echo "#include <SDL2/SDL.h>" > .myinclude
	echo "#include <SDL2/SDL_rect.h>" >> .myinclude
	echo "#include <SDL2/SDL_events.h>" >> .myinclude
	echo "#include <time.h>" >> .myinclude
	cat .myinclude main2.c > main3.c
	gcc -g main3.c -w -lSDL2 -lm
	rm -f .myinclude
	
run: sd
	./a.out

clean:
	rm -f main2.c main3.c
