#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

int main() {

	SDL_Window* window = NULL;

	SDL_Surface* screenSurface = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) < 0){

		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		exit(1);
	}

	window = SDL_CreateWindow("SDL Tutorial", 
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);

	if(window == NULL){
		printf("window could not be created! SDL Error: %s\n", SDL_GetError());
		exit(1);
	}


	screenSurface = SDL_GetWindowSurface(window);

	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xff, 0xff, 0xff));

	for(int i=0; i < 100; i++){

		struct SDL_Rect rect;
		rect = (struct SDL_Rect) {
			.x = (rand()%SCREEN_WIDTH),
			.y = (rand()%SCREEN_HEIGHT),
			.w = 10,
			.h = 10
		};

		SDL_FillRect(screenSurface, &rect, SDL_MapRGB(screenSurface->format, 0xaa, 0xaa, 0xaa));

		SDL_UpdateWindowSurface(window);

		SDL_Delay(200);

	}

	return 0;
}
