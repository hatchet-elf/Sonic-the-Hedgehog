const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <time.h>
#include "player.h"
#include "terrain.c"
#include "sonic_engine.c"

int main(int arc, char *argv[])
{

	int temp = 0;

	bool draw_collission_squares = false;

	SDL_Window *window = NULL;
	SDL_Surface *screensurface = NULL;
	SDL_Surface *charactersurface = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *sonicsprite = NULL;
	bool gameisrunning = true;

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("Video init failed\n");
		return 2;
	}

	window = SDL_CreateWindow("Sonic",
			5000,
			0,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
	);

	if(window == NULL)
	{
		printf("Couldn't create window\n");
		return 1;
	}

	// Initiate everything for SDL
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	SDL_Event event;
	charactersurface = IMG_Load("sonicsheet.png");
	sonicsprite = SDL_CreateTextureFromSurface(renderer, charactersurface);

	const Uint8 *keyboardstate;

	// Initiate everything for Sonic
	player sonic;
	player_init(&sonic);

	// init the terrain
	terrain *level;
	level = terrain_init();

	player_fall(&sonic);

/******* test code
	SDL_Rect all_sprites[7][7];
	 
	// fill the array with all the sprites from the spritesheet
	for(y = 0; y < 7; y++){
		for(x = 0; x < 7; x++){
			all_sprites[y][x].y = y * 65;
			all_sprites[y][x].x = x * 65;
			all_sprites[y][x].w = 65;
			all_sprites[y][x].h = 65;
		}
	}

*/
	while(gameisrunning)
{
		// handle keyboard input
		while(SDL_PollEvent(&event))
		{
			switch(event.type){
				case SDL_QUIT:
					gameisrunning = false;
					break;
					
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_s)
					{
						if(draw_collission_squares)
						{
							draw_collission_squares = false;
							break;
						}
					draw_collission_squares = true;
					}
					break;
			}

			keyboardstate = SDL_GetKeyboardState(NULL);
			
			if(keyboardstate[SDL_SCANCODE_RIGHT])
			{
				player_start_running_right(&sonic);
			}

			if(keyboardstate[SDL_SCANCODE_LEFT])
			{
				player_start_running_left(&sonic);
			}

			if(keyboardstate[SDL_SCANCODE_Q])
			{
				gameisrunning = false;
			}

			if(keyboardstate[SDL_SCANCODE_SPACE])
			{
				player_jump(&sonic);
			}
			
			if((!keyboardstate[SDL_SCANCODE_RIGHT] && !keyboardstate[SDL_SCANCODE_LEFT]) && 
					((sonic.current_x_action == RUNNINGLEFT) || (sonic.current_x_action == RUNNINGRIGHT)))
			{
				player_stand(&sonic);
			}
		}

		// get next sprite
		// move Sonic
		// check for any collissions
		player_get_next_sprite(&sonic);
		player_move(&sonic);

		if(terrain_collission(&sonic, level) == COL_NONE)
		{
			if(sonic.current_y_action != JUMPING)
			{
				player_fall(&sonic);
			}
		}
				

		// draw the terrain
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		terrain_draw(renderer, level);

		// draw the player
		if(sonic.current_y_action == JUMPING)
		{
			if(sonic.current_x_action == RUNNINGRIGHT)
			{
				SDL_RenderCopy(renderer, sonicsprite, &sonic.jumping[sonic.current_sprite_jump_index], &sonic.location);
			}
			if(sonic.current_x_action != RUNNINGRIGHT)
			{
				SDL_RenderCopyEx(renderer, sonicsprite, &sonic.jumping[sonic.current_sprite_jump_index], &sonic.location, 0, NULL, SDL_FLIP_HORIZONTAL);
			}
		}

		if(sonic.current_y_action != JUMPING)
		{
			// Copy the next Sprite to the renderer
			switch(sonic.current_x_action)
			{
				case RUNNINGRIGHT:
					SDL_RenderCopy(renderer, sonicsprite, &sonic.running[sonic.current_sprite_running_index], &sonic.location);
					break;

				case RUNNINGLEFT:
					SDL_RenderCopyEx(renderer, sonicsprite, &sonic.running[sonic.current_sprite_running_index], &sonic.location, 0, NULL, SDL_FLIP_HORIZONTAL);
					break;

				case STANDING:
					// Draw Sonic facing right or left
					if(sonic.left_or_right_before_standing == RUNNINGRIGHT)
					{
						SDL_RenderCopy(renderer, sonicsprite, &sonic.standing[sonic.current_sprite_standing_index], &sonic.location);
					}
					if(sonic.left_or_right_before_standing != RUNNINGRIGHT)
					{
						SDL_RenderCopyEx(renderer, sonicsprite, &sonic.standing[sonic.current_sprite_standing_index], &sonic.location, 0, NULL, SDL_FLIP_HORIZONTAL);
					}
					break;
			}
		}

		if(draw_collission_squares)
		{
			SDL_RenderDrawRect(renderer, &sonic.location);
		}

		SDL_RenderPresent(renderer);
	}
	
	SDL_FreeSurface(charactersurface);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);


	terrain_free(level);

	return 0;
}
