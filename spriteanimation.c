#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

SDL_Rect sonicrect[7][7];
SDL_Rect soniclocation;
SDL_Rect sonicrunning[7];

int initsonic();

int main(int arc, char *argv[]){

	int x, y;

	SDL_Window *window = NULL;
	SDL_Surface *screensurface = NULL;
	SDL_Surface *charactersurface = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *sonicstandingsprite = NULL;

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
	// fill the sonicrect array with all the sprites from the spritesheet
	for(y = 0; y < 7; y++){
		for(x = 0; x < 7; x++){
			sonicrect[y][x].y = y * 65;
			sonicrect[y][x].x = x * 65;
			sonicrect[y][x].w = 65;
			sonicrect[y][x].h = 65;
		}
	}

	soniclocation.x = 100;
	soniclocation.y = SCREEN_HEIGHT - sonicrect[0][0].h - 100;
	soniclocation.w = 100; //sonicrect[0][0].w;
	soniclocation.h = 100; //sonicrect[0][0].h;

	initsonic();

	sonicstandingsprite = SDL_CreateTextureFromSurface(renderer, charactersurface);

// Make Sonic run to the right
 while(soniclocation.x < SCREEN_WIDTH){
	for(y = 0; y < 8; y++){
		for(x = 0; x < 3; x++){
				SDL_RenderClear(renderer);
				SDL_RenderCopy(renderer, sonicstandingsprite, &sonicrunning[y], &soniclocation);
				SDL_RenderPresent(renderer);
				soniclocation.x += 10;
				SDL_Delay(20);
		}
	}
}

// Make Sonic run to the left
soniclocation.x = SCREEN_WIDTH;

 while(soniclocation.x > 0){
	for(y = 0; y < 8; y++){
		for(x = 0; x < 3; x++){
				SDL_RenderClear(renderer);
				SDL_RenderCopyEx(renderer, sonicstandingsprite, &sonicrunning[y], &soniclocation, 0, NULL, SDL_FLIP_HORIZONTAL);
				SDL_RenderPresent(renderer);
				soniclocation.x -= 10;
				SDL_Delay(20);
		}
	}
}

	SDL_FreeSurface(charactersurface);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
}

int initsonic(){
	int x, y;

	sonicrunning[0].y = 0;
	sonicrunning[0].x = 65 * 6;

	sonicrunning[1].y = 65;
	sonicrunning[1].x = 65 * 4;

	sonicrunning[2].y = 65;
	sonicrunning[2].x = 65 * 5;
	
	sonicrunning[3].y = 65;
	sonicrunning[3].x = 65 * 6;

	sonicrunning[4].y = 65 * 2;
	sonicrunning[4].x = 65;

	sonicrunning[5].y = 65 * 2;
	sonicrunning[5].x = 0;

	sonicrunning[6].y = 65 * 2;
	sonicrunning[6].x = 65;

	sonicrunning[7].y = 65 * 2;
	sonicrunning[7].x = 65 * 2;

	for(y = 0; y < 8; y++){
		sonicrunning[y].h = 65;
		sonicrunning[y].w = 65;
	}
	
}