#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#include "getkey.h"

int ftoi(float f){ return (int)f; }

//https://www.libsdl.org/release/SDL-1.2.15/docs/html/guideinputkeyboard.html

char getkey(){
	
	SDL_Event e;
	
	static int yvel = 0;
	
	if(SDL_PollEvent(&e) != 0){
		
		if(e.type == SDL_QUIT){ exit(1); }
		
		if(e.type == SDL_KEYDOWN){
		
			switch(e.key.keysym.sym){
				
				case SDLK_UP: 
					yvel = -1;
					break;
					
				case SDLK_DOWN: 
					yvel = 1;
					break;
					
				case SDLK_q: return 'q';
			}
		}
		
		if(e.type == SDL_KEYUP){
		
			switch(e.key.keysym.sym){
				
				case SDLK_UP: 
					if(yvel < 0){ yvel = 0; }
					break;
					
				case SDLK_DOWN: 
					if(yvel > 0){ yvel = 0; }
					break;
			}
		}
	}
	
	if(yvel < 0){ return 'u'; }
	if(yvel > 0){ return 'd'; }
	
	return ' ';
}
