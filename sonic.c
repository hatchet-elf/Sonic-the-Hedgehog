const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <time.h>
#include "sonic_functions.c"

int main(int arc, char *argv[]){

	int x, y;

	SDL_Window *window = NULL;
	SDL_Surface *screensurface = NULL;
	SDL_Surface *charactersurface = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *sonicsprite = NULL;
	bool gameisrunning = true;

	if(SDL_Init(SDL_INIT_VIDEO) == 0){
		printf("Video init succesful\n");
	}else{
		printf("Vido init failed\n");
	return 2;
	}

	window = SDL_CreateWindow("Sonic",
			5000,
			0,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
	);

	if(window == NULL){
		printf("Couldn't create window\n");
		return 1;
	}

	// Initiate everything for SDl
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	SDL_Event event;
	sonic.time_between_running_frames = SDL_GetPerformanceFrequency() / RUNNING_SPEED;
	charactersurface = IMG_Load("sonicsheet.png");
	sonicsprite = SDL_CreateTextureFromSurface(renderer, charactersurface);

	// Initiate everything for Sonic
	sonic_init();
	sonic.last_frame_timer = SDL_GetPerformanceCounter();

	while(gameisrunning){
		while(SDL_PollEvent(&event)){
			switch(event.type){
				case SDL_QUIT:
					gameisrunning = false;
					break;

				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){
						case SDLK_RIGHT:
							sonic_start_running_right();
							break;

						case SDLK_UP:
							sonic_stand();
							break;

						case SDLK_LEFT:
							sonic_start_running_left();
							break;

						case SDLK_q:
							gameisrunning = false;
							break;

						default:
							break;
					}

				default:
					break;
			}
		}

		sonic_get_next_sprite();
		sonic_move();

		SDL_RenderClear(renderer);

		// copy whichever is the correct sprite to the renderer
		switch(sonic.current_action){
			case RUNNINGRIGHT:
				SDL_RenderCopy(renderer, sonicsprite, &sonic.running_right[sonic.current_sprite_index], &sonic.location);
				break;

			case STANDING:
				SDL_RenderCopy(renderer, sonicsprite, &sonic.standing[sonic.current_sprite_index], &sonic.location);
				break;

			case RUNNINGLEFT:
				SDL_RenderCopyEx(renderer, sonicsprite, &sonic.running_right[sonic.current_sprite_index], &sonic.location, 0, NULL, SDL_FLIP_HORIZONTAL);
				break;

		}

		SDL_RenderPresent(renderer);
	}
	
	SDL_FreeSurface(charactersurface);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
}

