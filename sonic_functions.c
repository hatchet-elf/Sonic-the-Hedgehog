#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <time.h>

#define RUNNING_SPEED 1.5 
#define TIME_BETWEEN_RUNNING_FRAMES 50
#define TIME_BETWEEN_STANDING_FRAMES 100
#define TIME_BETWEEN_JUMPING_FRAMES 50

#define GRAVITY .001
#define JUMP_VELOCITY 1.8

#define RUNNINGRIGHT 0
#define RUNNINGLEFT 1
#define STARTRUNNING 2
#define STOPRUNNING 3
#define STANDING 4
#define JUMPING 5
#define TOPOFJUMP 6
#define	STARTFALLING 7
#define FALLING 8
#define ONTHEGROUND 9

typedef struct {
	// stores players location
	SDL_Rect location;

	// the below are arrays of frames for different actions
	SDL_Rect running[8];
	SDL_Rect standing[5];
	SDL_Rect jumping[6];

	// used to show animation frames with correct timing
	Uint64 last_frame_timer;

	// the current action is what Sonic is doing
	// 	ie: running, standing or jumping
	// 	the reason for separate x and y actions is to allow for two actions at the same time to take place
	// 	x or y indicates if the action is affecting the player on the x or y plane
	// 	the x action is for running or standing
	// 	the y actions are for jumping or falling
	int current_x_action;
	int current_y_action;

	// the index of the current sprite in the current action
	//int current_sprite_index;
	int current_sprite_jump_index;
	int current_sprite_running_index;
	int current_sprite_standing_index;

	// used to store the location and time when the player starts running
	int running_start_x;
	Uint32 running_start_x_time;

	// Used for when the player jumps
	Uint32 jump_y;
	Uint32 jump_start_time;
	float jump_velocity;

	// Used for when the player starts running
	Uint32 run_x;
	Uint32 run_start_time;

	// This is set to the below values when the stand() function is called
	// RUNNINGRIGHT - the player was facing to the right before stand() was called
	// RUNNINGLEFT - the player was facing to the left before stand() was called
	int left_or_right_before_standing;

} player;


int player_start_running_right(player *sprite){
	if(sprite->current_x_action == RUNNINGRIGHT){
		return 0;
	}
	sprite->current_x_action = RUNNINGRIGHT;

	// store where Sonic is when he starts running
	sprite->running_start_x = sprite->location.x;
	sprite->running_start_x_time = SDL_GetTicks();
	sprite->current_sprite_running_index = 0;


	return 0;

}

int player_start_running_left(player *sprite){
	if(sprite->current_x_action == RUNNINGLEFT){
		return 0;
	}

	sprite->current_x_action = RUNNINGLEFT;

	// store where Sonic is when he starts running
	sprite->running_start_x = sprite->location.x;
	sprite->running_start_x_time = SDL_GetTicks();
	sprite->current_sprite_running_index = 0;

	return 0;
}

int player_jump(player *sprite){
	// if the player is not on the ground then don't do anything
	if(sprite->location.y != terrain_ground_level()){
		return 0;
	}


	sprite->current_y_action = JUMPING;

	// store where Sonic is when he starts 
	sprite->jump_y = sprite->location.y;
	sprite->jump_start_time = SDL_GetTicks();
	sprite->current_sprite_jump_index = 0;

	sprite->jump_velocity = JUMP_VELOCITY;

	return 0;

}

// returns RUNNINGRIGHT if the player should be facing to the right
// returns RUNNINGLEFT if the player should be facing left
// This is so that when the function is called you have an easy way of knowing which way the player was facing
// the variable sprite->left_or_right_before_standing is also set
int player_stand(player *sprite){

	if(sprite->current_y_action == (JUMPING || FALLING)){
		return 0;
	}

	sprite->left_or_right_before_standing = sprite->current_x_action;

	sprite->current_x_action = STANDING;
	sprite->current_sprite_standing_index = 0;

	return sprite->left_or_right_before_standing;
}

int player_get_next_sprite(player *sprite){
	Uint32 elapsed_time;
	Uint32 end;

	end = SDL_GetTicks();
	elapsed_time = end - sprite->last_frame_timer;
	
	switch(sprite->current_y_action){
		case JUMPING:
			if(elapsed_time > TIME_BETWEEN_JUMPING_FRAMES){
				sprite->current_sprite_jump_index++;

				if(sprite->current_sprite_jump_index >= 4){
					       sprite->current_sprite_jump_index = 0;
					} 

				sprite->last_frame_timer = SDL_GetTicks();
			}
			break;

		case FALLING:
			break;

		return 0;
	}

	switch(sprite->current_x_action){
		case RUNNINGRIGHT:

			// This code works by calculating the time from the last frame and only animating the next frame
			// once TIME_BETWEEN_RUNNING_FRAMES has passed
			// This is using SDL_GetTicks() which is accurate to a millisecond
			// So it will be out slightly however it should not be noticeable to the player
			if(elapsed_time > TIME_BETWEEN_RUNNING_FRAMES){
				if(sprite->current_sprite_running_index == 7){
					       sprite->current_sprite_running_index = 0;
					} else {
						sprite->current_sprite_running_index++;
					}
				sprite->last_frame_timer = SDL_GetTicks();
			}
			break;

		case RUNNINGLEFT:

			if(elapsed_time > TIME_BETWEEN_RUNNING_FRAMES){
				if(sprite->current_sprite_running_index == 7){
					       sprite->current_sprite_running_index = 0;
					} else {
						sprite->current_sprite_running_index++;
					}
				sprite->last_frame_timer = SDL_GetTicks();
			}
			break;

		case STANDING:
			if(elapsed_time > TIME_BETWEEN_STANDING_FRAMES){
				if(sprite->current_sprite_standing_index == 4){
					       sprite->current_sprite_standing_index = 0;
					} else {
						sprite->current_sprite_standing_index++;
					}

				sprite->last_frame_timer = SDL_GetTicks();
			}
		break;

		return 0;
	}


	return 0;
}



int player_move(player *sprite){
	float y = 0;
	float x = 0;
	Uint32 end_jump_time;
	Uint32 elapsed_jump_time;

	Uint32 end_run_time;
	Uint32 elapsed_run_time;

	float dt;

	// If statements are used rather than a switch statement
	// The reason is that there may be more than one statement that gets used
	// For example: jumping whilst running
	if(sprite->current_x_action == RUNNINGRIGHT){

		end_run_time = SDL_GetTicks();
		elapsed_run_time = end_run_time - sprite->running_start_x_time;
		x = (float)sprite->running_start_x + (float)(elapsed_run_time / RUNNING_SPEED);

		sprite->location.x = (int)x;

	}

	if(sprite->current_x_action == RUNNINGLEFT){

		end_run_time = SDL_GetTicks();
		elapsed_run_time = end_run_time - sprite->running_start_x_time;
		x = (float)sprite->running_start_x - (float)(elapsed_run_time / RUNNING_SPEED);

		sprite->location.x = (int)x;
	}

	if(sprite->current_y_action == JUMPING){

		end_jump_time = SDL_GetTicks();
		elapsed_jump_time = end_jump_time - sprite->jump_start_time;

		// This code is the jump
		// Very proud of this as I had to learn math that I didn't know existed
		y = (float)sprite->jump_y - (float)(elapsed_jump_time * sprite->jump_velocity);
		sprite->jump_velocity = sprite->jump_velocity - GRAVITY;
		sprite->location.y = (int)y;

		// When the player gets to the ground then stand
		if(sprite->location.y > terrain_on_the_ground()){
			       sprite->current_y_action = STANDING;
			       sprite->location.y = terrain_on_the_ground();
		}
	}

	return 0;
}

int player_init(player *sprite){
	int x, y;

	memset(sprite, 0, sizeof(sprite));

	sprite->last_frame_timer = SDL_GetTicks();
	sprite->current_x_action = STANDING;
	sprite->current_y_action = ONTHEGROUND;

	// setup all the frames for sonic running
	sprite->running[0].y = 0;
	sprite->running[0].x = 65 * 6;
	sprite->running[0].h = 65;
	sprite->running[0].w = 65;

	sprite->running[1].y = 65;
	sprite->running[1].x = 65 * 4;
	sprite->running[1].h = 65;
	sprite->running[1].w = 65;

	sprite->running[2].y = 65;
	sprite->running[2].x = 65 * 5;
	sprite->running[2].h = 65;
	sprite->running[2].w = 65;
	
	sprite->running[3].y = 65;
	sprite->running[3].x = 65 * 6;
	sprite->running[3].h = 65;
	sprite->running[3].w = 65;

	sprite->running[4].y = 65 * 2;
	sprite->running[4].x = 65;
	sprite->running[4].h = 65;
	sprite->running[4].w = 65;

	sprite->running[5].y = 65 * 2;
	sprite->running[5].x = 0;
	sprite->running[5].h = 65;
	sprite->running[5].w = 65;

	sprite->running[6].y = 65 * 2;
	sprite->running[6].x = 65;
	sprite->running[6].h = 65;
	sprite->running[6].w = 65;

	sprite->running[7].y = 65 * 2;
	sprite->running[7].x = 65 * 2;
	sprite->running[7].h = 65;
	sprite->running[7].w = 65;

	// setup the frames for the player standing
	sprite->standing[0].y = 0;
	sprite->standing[0].x = -1;
	sprite->standing[0].h = 65;
	sprite->standing[0].w = 65;

	sprite->standing[1].y = 0;
	sprite->standing[1].x = 65 - 2;
	sprite->standing[1].h = 65;
	sprite->standing[1].w = 65;

	sprite->standing[2].y = 0;
	sprite->standing[2].x = 65 * 2 - 3;
	sprite->standing[2].h = 65;
	sprite->standing[2].w = 65;

	sprite->standing[3].y = 0;
	sprite->standing[3].x = 65 * 3 - 4;
	sprite->standing[3].h = 65;
	sprite->standing[3].w = 65;

	sprite->standing[4].y = 0;
	sprite->standing[4].x = 65 * 4 - 5;
	sprite->standing[4].h = 65;
	sprite->standing[4].w = 65;
	
	// setup the frames for the player jumping
	sprite->jumping[0].y = 65 * 4;
	sprite->jumping[0].x = 65;
	sprite->jumping[0].h = 65;
	sprite->jumping[0].w = 65;

	sprite->jumping[1].y = 65 * 4;
	sprite->jumping[1].x = 65 * 2;
	sprite->jumping[1].h = 65;
	sprite->jumping[1].w = 65;

	sprite->jumping[2].y = 65 * 4;
	sprite->jumping[2].x = 65 * 3;
	sprite->jumping[2].h = 65;
	sprite->jumping[2].w = 65;

	sprite->jumping[3].y = 65 * 4;
	sprite->jumping[3].x = 65 * 4;
	sprite->jumping[3].h = 65;
	sprite->jumping[3].w = 65;

	// have sonic start at the lower left of the screen
	sprite->location.x = 100;
	sprite->location.y = terrain_ground_level();
	sprite->location.w = 100; //sonicrect[0][0].w;
	sprite->location.h = 100; //sonicrect[0][0].h;

}
