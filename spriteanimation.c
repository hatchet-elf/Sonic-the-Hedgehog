#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <time.h>

// Sonic's running speed in pixels per second
#define SONIC_RUNSPEED 1000
#define FRAMES_PER_SECOND 1000000000 / SONIC_RUNSPEED
#define TIME_BETWEEN_SPRITES 40000000

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;


struct {
	// this is all frames from the sprite sheet
	SDL_Rect sonicrect[7][7];

	// stores sonics location
	SDL_Rect location;

	// this is the an array of the frames for sonic running
	SDL_Rect running[8];

	// timer for Sonic running
	struct timespec current_running_timer;
	struct timespec previous_running_timer;
	uint64_t time_spent_running;

	// timer for shoing the next frame
	struct timespec current_nextframe_timer;
	struct timespec previous_nextframe_timer;
	uint64_t nextframe_time;

	int nexty;
	int nextx;

	int current_sprite_index;

} sonic;

int initsonic();

int sonic_move();

int sonic_next_sprite();

int main(int arc, char *argv[]){

	int x, y;

	SDL_Window *window = NULL;
	SDL_Surface *screensurface = NULL;
	SDL_Surface *charactersurface = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *sonicsprite = NULL;

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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	SDL_Event *event;

	charactersurface = IMG_Load("sonicsheet.png");

	initsonic();

	sonicsprite = SDL_CreateTextureFromSurface(renderer, charactersurface);

	while(sonic.location.x < SCREEN_WIDTH){

		// these clocks look after sonic showing the next sprite character
		clock_gettime(CLOCK_MONOTONIC, &sonic.current_nextframe_timer);

		sonic.nextframe_time = 1000000000 * (sonic.current_nextframe_timer.tv_sec - sonic.previous_nextframe_timer.tv_sec) + 
			sonic.current_nextframe_timer.tv_nsec - sonic.previous_nextframe_timer.tv_nsec;

		
		// this draws the next sprite at the right time
		if(sonic.nextframe_time > TIME_BETWEEN_SPRITES){
			sonic_next_sprite();

			clock_gettime(CLOCK_MONOTONIC, &sonic.current_nextframe_timer);
			sonic.previous_nextframe_timer.tv_sec = sonic.current_nextframe_timer.tv_sec;
			sonic.previous_nextframe_timer.tv_nsec = sonic.current_nextframe_timer.tv_nsec;
		}
		
		// these clock statements look after sonic moving on screen
		clock_gettime(CLOCK_MONOTONIC, &sonic.current_running_timer);

		sonic.time_spent_running = 1000000000 * (sonic.current_running_timer.tv_sec - sonic.previous_running_timer.tv_sec) + 
			sonic.current_running_timer.tv_nsec - sonic.previous_running_timer.tv_nsec;

		// this moves sonic 
		if(sonic.time_spent_running > FRAMES_PER_SECOND){
			sonic_move();
					
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, sonicsprite, &sonic.running[sonic.current_sprite_index], &sonic.location);
			SDL_RenderPresent(renderer);

			clock_gettime(CLOCK_MONOTONIC, &sonic.current_running_timer);
			sonic.previous_running_timer.tv_sec = sonic.current_running_timer.tv_sec;
			sonic.previous_running_timer.tv_nsec = sonic.current_running_timer.tv_nsec;
		}
	}
	
	SDL_FreeSurface(charactersurface);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
	}

int sonic_next_sprite(){

	if(sonic.current_sprite_index == 7){
	       sonic.current_sprite_index = 0;
	} else {
		sonic.current_sprite_index++;
	}

	return 0;
}

int sonic_move(){
	sonic.location.x += 1;

	return 0;
}


int initsonic(){
	int x, y;

	sonic.nexty = 0;
	sonic.nextx = 65 / 6;

	// setup all the frames for sonic running
	sonic.running[0].y = 0;
	sonic.running[0].x = 65 * 6;

	sonic.running[1].y = 65;
	sonic.running[1].x = 65 * 4;

	sonic.running[2].y = 65;
	sonic.running[2].x = 65 * 5;
	
	sonic.running[3].y = 65;
	sonic.running[3].x = 65 * 6;

	sonic.running[4].y = 65 * 2;
	sonic.running[4].x = 65;

	sonic.running[5].y = 65 * 2;
	sonic.running[5].x = 0;

	sonic.running[6].y = 65 * 2;
	sonic.running[6].x = 65;

	sonic.running[7].y = 65 * 2;
	sonic.running[7].x = 65 * 2;

	for(y = 0; y < 8; y++){
		sonic.running[y].h = 65;
		sonic.running[y].w = 65;
	}

	// have sonic start at the lower left of the screen
	sonic.location.x = 100;
	sonic.location.y = SCREEN_HEIGHT - sonic.sonicrect[0][0].h - 100;
	sonic.location.w = 100; //sonicrect[0][0].w;
	sonic.location.h = 100; //sonicrect[0][0].h;

	// fill the sonicrect array with all the sprites from the spritesheet
	for(y = 0; y < 7; y++){
		for(x = 0; x < 7; x++){
			sonic.sonicrect[y][x].y = y * 65;
			sonic.sonicrect[y][x].x = x * 65;
			sonic.sonicrect[y][x].w = 65;
			sonic.sonicrect[y][x].h = 65;
		}
	}

}
