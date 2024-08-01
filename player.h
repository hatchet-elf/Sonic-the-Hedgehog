#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <time.h>

#define PLAYER_HEIGHT 100
#define PLAYER_WIDTH 100
#define SONIC_HEIGHT 100
#define SONIC_WIDTH 100

#define RUNNING_SPEED 1.5 
#define TIME_BETWEEN_RUNNING_FRAMES 50
#define TIME_BETWEEN_STANDING_FRAMES 100
#define TIME_BETWEEN_JUMPING_FRAMES 50

#define GRAVITY .001
#define JUMP_VELOCITY 1.9
#define FALL_VELOCITY 1.3

#define COYOTE_TIME 50
#define JUMP_BUFFER 50

#define RUNNINGRIGHT 0
#define RUNNINGLEFT 1
#define STARTRUNNING 2
#define STOPRUNNING 3
#define STANDING 4
#define JUMPING 5
#define TOPOFJUMP 6
#define STARTFALLING 7
#define FALLING 8
typedef struct
{
        // stores players location
        SDL_Rect location;

        // the below are arrays of frames for different actions
        SDL_Rect running[8];
        SDL_Rect standing[5];
        SDL_Rect jumping[6];

        // used to show animation frames with correct timing
        Uint64 last_frame_timer;

        // the current action is what Sonic is doing
        //      ie: running, standing or jumping
        //      the reason for separate x and y actions is to allow for two actions at the same time to take place
        //      x or y indicates if the action is affecting the player on the x or y plane
        //      the x action is for running or standing
        //      the y actions are for jumping or falling
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

	Uint32 fall_y;
	Uint32 fall_start_time;
	float fall_velocity;

        // This is set to the below values when the stand() function is called
        // RUNNINGRIGHT - the player was facing to the right before stand() was called
        // RUNNINGLEFT - the player was facing to the left before stand() was called
        int left_or_right_before_standing;

	// Used for Coyote time
	Uint32 on_a_platform;

	Uint32 jump_buffer;
	bool jump_button_hit;


} player;
