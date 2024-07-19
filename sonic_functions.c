#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <time.h>

#define RUNNING_SPEED 1.5 
#define TIME_BETWEEN_RUNNING_FRAMES 50
#define TIME_BETWEEN_STANDING_FRAMES 80

#define JUMP_HEIGHT 260
#define JUMP_SPEED 1.2

#define FALL_SPEED 1.2

#define MAX_X_SPEED 1

// this is by how much the inertia will be increased when Sonic starts running
#define RUNNING_INERTIA_INCREASE 1.9
#define RUNNING_RIGHT_INERTIA_START 1.01 
#define RUNNING_LEFT_INERTIA_START -1.01 


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


struct {
	// this is all frames from the sprite sheet
	SDL_Rect sonicrect[7][7];

	// stores sonics location
	SDL_Rect location;

	// this is the an array of the frames for sonic running
	SDL_Rect running_right[8];
	SDL_Rect standing[5];

	// used to show animation frames with correct timing
	Uint64 last_frame_timer;

	// the current action is what Sonic is doing
	// 	ie: running, standing or jumping
	int current_x_action;
	int current_y_action;

	// the index of the current sprite in the current action
	int current_sprite_index;

	// used to draw sonic at the correct location
	int running_start_x;
	Uint32 running_start_x_time;

	Uint32 jump_y;
	Uint32 jump_start_time;

	Uint32 fall_y;
	Uint32 fall_start_time;


} sonic;

int sonic_start_running_right(){
	if(sonic.current_x_action = RUNNINGRIGHT){
		return 0;
	}

	sonic.current_x_action = RUNNINGRIGHT;

	// store where Sonic is when he starts running
	sonic.running_start_x = sonic.location.x;
	sonic.running_start_x_time = SDL_GetTicks();

	return 0;

}

int sonic_start_running_left(){
	if(sonic.current_x_action == RUNNINGLEFT){
		return 0;
	}

	sonic.current_x_action = RUNNINGLEFT;

	// store where Sonic is when he starts running
	sonic.running_start_x = sonic.location.x;
	sonic.running_start_x_time = SDL_GetTicks();

	sonic.current_sprite_index = 0;

	return 0;
}

int sonic_jump(){
	if(sonic.location.y != ground_level()){
		return 0;
	}


	sonic.current_y_action = JUMPING;

	// store where Sonic is when he starts 
	sonic.jump_y = sonic.location.y;
	sonic.jump_start_time = SDL_GetTicks();
	sonic.current_sprite_index = 0;

	return 0;

}

int sonic_fall(){
	sonic.current_y_action = FALLING;

	// store where Sonic is when he starts 
	sonic.fall_y = sonic.location.y;
	sonic.fall_start_time = SDL_GetTicks();
	sonic.current_sprite_index = 0;

	return 0;

}



int sonic_stand(){
	sonic.current_x_action = STANDING;
	sonic.current_y_action = ONTHEGROUND;
	sonic.current_sprite_index = 0;

	return 0;
}


int sonic_get_next_sprite(){
	Uint32 elapsed_time;
	Uint32 end;

	end = SDL_GetTicks();
	elapsed_time = end - sonic.last_frame_timer;

	
	switch(sonic.current_x_action){
		case RUNNINGRIGHT:

			if(elapsed_time > TIME_BETWEEN_RUNNING_FRAMES){
				if(sonic.current_sprite_index == 7){
					       sonic.current_sprite_index = 0;
					} else {
						sonic.current_sprite_index++;
					}
				sonic.last_frame_timer = SDL_GetTicks();
			}
			break;

		case RUNNINGLEFT:

			if(elapsed_time > TIME_BETWEEN_RUNNING_FRAMES){
				if(sonic.current_sprite_index == 7){
					       sonic.current_sprite_index = 0;
					} else {
						sonic.current_sprite_index++;
					}
				sonic.last_frame_timer = SDL_GetTicks();
			}
			break;

		case STANDING:
		if(elapsed_time > TIME_BETWEEN_STANDING_FRAMES){
			if(sonic.current_sprite_index == 4){
				       sonic.current_sprite_index = 0;
				} else {
					sonic.current_sprite_index++;
				}
			sonic.last_frame_timer = SDL_GetTicks();
		}
		break;

	}

	switch(sonic.current_y_action){
		case JUMPING:
			break;

		case FALLING:
			break;

	}

	return 0;
}



int sonic_move(){
	int elapsed_running_time;
	float tempx = 0;
	float y = 0;
	Uint32 end_time;
	Uint32 elapsed_time;

	Uint32 end_jump_time;
	Uint32 elapsed_jump_time;

	Uint32 end_fall_time;
	Uint32 elapsed_fall_time;

	// if statements are used rather than a switch statement
	// the reason is that there may be more than one statement that gets used
	// for example: jumping whilst running
	if(sonic.current_x_action == RUNNINGRIGHT){

		end_time = SDL_GetTicks();
		elapsed_time = end_time - sonic.running_start_x_time;

		tempx = (float)sonic.running_start_x + (float)(elapsed_time / 1.5);
		sonic.location.x = (int)tempx;

	}

	if(sonic.current_x_action == RUNNINGLEFT){

		end_time = SDL_GetTicks();
		elapsed_time = end_time - sonic.running_start_x_time;

		tempx = (float)sonic.running_start_x - (float)(elapsed_time / 1.5);
		sonic.location.x = (int)tempx;
	}

	if(sonic.current_y_action == JUMPING){

		end_jump_time = SDL_GetTicks();
		elapsed_jump_time = end_jump_time - sonic.jump_start_time;

		y = (float)sonic.jump_y - (float)(elapsed_jump_time * JUMP_SPEED);
		sonic.location.y = (int)y;
		
		if(sonic.location.y <  (sonic.jump_y - JUMP_HEIGHT)){
			sonic_fall();
		}
	}

	if(sonic.current_y_action == FALLING){

		end_fall_time = SDL_GetTicks();
		elapsed_fall_time = end_fall_time - sonic.fall_start_time;

		y = (float)sonic.fall_y + (float)(elapsed_fall_time * FALL_SPEED);
		sonic.location.y = (int)y;
		
		if(sonic.location.y > ground_level()){
			       sonic.current_y_action = STANDING;
			       sonic.location.y = ground_level();
		}
	}


	return 0;
}

int sonic_init(){
	int x, y;

	sonic.last_frame_timer = SDL_GetTicks();
	sonic.current_x_action = STANDING;
	sonic.current_y_action = ONTHEGROUND;

	// setup all the frames for sonic running
	sonic.running_right[0].y = 0;
	sonic.running_right[0].x = 65 * 6;

	sonic.running_right[1].y = 65;
	sonic.running_right[1].x = 65 * 4;

	sonic.running_right[2].y = 65;
	sonic.running_right[2].x = 65 * 5;
	
	sonic.running_right[3].y = 65;
	sonic.running_right[3].x = 65 * 6;

	sonic.running_right[4].y = 65 * 2;
	sonic.running_right[4].x = 65;

	sonic.running_right[5].y = 65 * 2;
	sonic.running_right[5].x = 0;

	sonic.running_right[6].y = 65 * 2;
	sonic.running_right[6].x = 65;

	sonic.running_right[7].y = 65 * 2;
	sonic.running_right[7].x = 65 * 2;

	for(y = 0; y < 8; y++){
		sonic.running_right[y].h = 65;
		sonic.running_right[y].w = 65;
	}

	// setup the frames for sonic standing
	sonic.standing[0].y = 0;
	sonic.standing[0].x = 0;
	sonic.standing[0].h = 65;
	sonic.standing[0].w = 65;

	sonic.standing[1].y = 0;
	sonic.standing[1].x = 65;
	sonic.standing[1].h = 65;
	sonic.standing[1].w = 65;

	sonic.standing[2].y = 0;
	sonic.standing[2].x = 65 * 2;
	sonic.standing[2].h = 65;
	sonic.standing[2].w = 65;

	sonic.standing[3].y = 0;
	sonic.standing[3].x = 65 * 3;
	sonic.standing[3].h = 65;
	sonic.standing[3].w = 65;

	sonic.standing[4].y = 0;
	sonic.standing[4].x = 65 * 4;
	sonic.standing[4].h = 65;
	sonic.standing[4].w = 65;

	// have sonic start at the lower left of the screen
	sonic.location.x = 100;
	sonic.location.y = ground_level();
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
