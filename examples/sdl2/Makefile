SOURCES = main.c main2.dg

#Some adjustment to the sd-generated main2.c file
#is required to get this example working. 
#It is a proof of concept of smalldragon C interop.

#considered for the future:
# - passthrough #include directives
# - externc struct declarations
#   - externc struct MyStruct "MY_STRUCT"

c: main.c
	#c variant
	gcc main.c -w -lSDL2 

sd: main2.dg
	#smalldragon variant
	sd main2.dg
	echo "#include <SDL2/SDL.h>" > .myinclude
	cat .myinclude main2.c > main3.c
	gcc main3.c -w -lSDL2 
	rm -f .myinclude

