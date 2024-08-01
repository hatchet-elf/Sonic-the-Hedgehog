int player_start_running_right(player *sprite)
{
	if(sprite->current_x_action == RUNNINGRIGHT)
	{
		return 0;
	}
	sprite->current_x_action = RUNNINGRIGHT;

	// store where the player is when they starts running
	sprite->running_start_x = sprite->location.x;
	sprite->running_start_x_time = SDL_GetTicks();
	sprite->current_sprite_running_index = 0;

	return 0;
}

int player_start_running_left(player *sprite)
{
	if(sprite->current_x_action == RUNNINGLEFT)
	{
		return 0;
	}

	sprite->current_x_action = RUNNINGLEFT;

	// store where the player is when they start running along
	sprite->running_start_x = sprite->location.x;
	sprite->running_start_x_time = SDL_GetTicks();
	sprite->current_sprite_running_index = 0;

	return 0;
}

int player_jump(player *sprite){
	
	// this is for jump buffering
	sprite->jump_button_hit = true;
	sprite->jump_buffer = SDL_GetTicks();

	// Coyote time
	Uint32 coyote_timer = SDL_GetTicks() - sprite->on_a_platform;
	if(coyote_timer > COYOTE_TIME){
		return 0;
	}


	if(sprite->current_y_action == JUMPING){
		return 0;
	}
	sprite->current_y_action = JUMPING;

	// store where the player is when they start
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

// This is called whenever the player is not on a platform or jumping
int player_fall(player *sprite)
{
	if(sprite->current_y_action == JUMPING)
	{
		return 0;
	}

	sprite->current_y_action = FALLING;
	sprite->fall_y = sprite->location.y;
	sprite->fall_start_time = SDL_GetTicks();

	sprite->fall_velocity = FALL_VELOCITY;

	return 0;
}

int player_get_next_sprite(player *sprite)
{
	Uint32 elapsed_time;
	Uint32 end;

	end = SDL_GetTicks();
	elapsed_time = end - sprite->last_frame_timer;
	
	switch(sprite->current_y_action)
	{
		case JUMPING:
			if(elapsed_time > TIME_BETWEEN_JUMPING_FRAMES)
			{
				sprite->current_sprite_jump_index++;
				if(sprite->current_sprite_jump_index >= 4)
				{
					       sprite->current_sprite_jump_index = 0;
				} 

				sprite->last_frame_timer = SDL_GetTicks();
			}
			break;

		case FALLING:
			break;

		return 0;
	}

	switch(sprite->current_x_action)
	{
		case RUNNINGRIGHT:

			// This code works by calculating the time from the last frame and only animating the next frame
			// once TIME_BETWEEN_RUNNING_FRAMES has passed
			// This is using SDL_GetTicks() which is accurate to a millisecond
			// So it will be out slightly however it should not be noticeable to the player
			if(elapsed_time > TIME_BETWEEN_RUNNING_FRAMES)
			{
				sprite->current_sprite_running_index++;
				if(sprite->current_sprite_running_index > 7)
				{
					       sprite->current_sprite_running_index = 0;
				} 

				sprite->last_frame_timer = SDL_GetTicks();
			}
			break;

		case RUNNINGLEFT:

			if(elapsed_time > TIME_BETWEEN_RUNNING_FRAMES)
			{
				sprite->current_sprite_running_index++;
				if(sprite->current_sprite_running_index > 7)
				{
				       sprite->current_sprite_running_index = 0;
				} 

				sprite->last_frame_timer = SDL_GetTicks();
			}
			break;

		case STANDING:
			if(elapsed_time > TIME_BETWEEN_STANDING_FRAMES)
			{
				sprite->current_sprite_standing_index++;
				if(sprite->current_sprite_standing_index == 4)
				{
				       sprite->current_sprite_standing_index = 0;
				}

				sprite->last_frame_timer = SDL_GetTicks();
			}
		break;

		return 0;
	}
	return 0;
}


int player_move(player *sprite)
{
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
	if(sprite->current_x_action == RUNNINGRIGHT)
	{

		end_run_time = SDL_GetTicks();
		elapsed_run_time = end_run_time - sprite->running_start_x_time;

		x = (float)sprite->running_start_x + (float)(elapsed_run_time / RUNNING_SPEED);

		sprite->location.x = (int)x;
	}

	if(sprite->current_x_action == RUNNINGLEFT)
	{
		end_run_time = SDL_GetTicks();
		elapsed_run_time = end_run_time - sprite->running_start_x_time;
		x = (float)sprite->running_start_x - (float)(elapsed_run_time / RUNNING_SPEED);

		sprite->location.x = (int)x;
	}

	if(sprite->current_y_action == JUMPING)
	{
		end_jump_time = SDL_GetTicks();
		elapsed_jump_time = end_jump_time - sprite->jump_start_time;

		// This code is the jump
		y = (float)sprite->jump_y - (float)(elapsed_jump_time * sprite->jump_velocity);
		sprite->jump_velocity = sprite->jump_velocity - GRAVITY;
		sprite->location.y = (int)y;	

	}

	if(sprite->current_y_action == FALLING)
	{
		sprite->location.y = sprite->location.y + 1;
	}
	return 0;
}

int player_init(player *sprite)
{
	int x, y;

	// edit these two lines to change the size of the sprite for the game
	int sprite_height = 100;
	int sprite_width = 100;

	memset(sprite, 0, sizeof(sprite));

	sprite->last_frame_timer = SDL_GetTicks();
	sprite->current_x_action = RUNNINGRIGHT;

	// setup all the frames for running
	sprite->running[0].y = 6;
	sprite->running[0].x = 392;
	sprite->running[0].h = 47;
	sprite->running[0].w = 44;

	sprite->running[1].y = 70;
	sprite->running[1].x = 263;
	sprite->running[1].h = 47;
	sprite->running[1].w = 44;

	sprite->running[2].y = 70;
	sprite->running[2].x = 328;
	sprite->running[2].h = 47;
	sprite->running[2].w = 44;
	
	sprite->running[3].y = 74;
	sprite->running[3].x = 389;
	sprite->running[3].h = 47;
	sprite->running[3].w = 44;

	sprite->running[4].y = 137;
	sprite->running[4].x = 69;
	sprite->running[4].h = 47;
	sprite->running[4].w = 44;

	sprite->running[5].y = 137;
	sprite->running[5].x = 5;
	sprite->running[5].h = 47;
	sprite->running[5].w = 44;

	sprite->running[6].y = 137;
	sprite->running[6].x = 69;
	sprite->running[6].h = 47;
	sprite->running[6].w = 44;

	sprite->running[7].y = 140;
	sprite->running[7].x = 134;
	sprite->running[7].h = 47;
	sprite->running[7].w = 44;

	// setup the frames for the player standing
	sprite->standing[0].y = 12;
	sprite->standing[0].x = 9;
	sprite->standing[0].h = 47;
	sprite->standing[0].w = 44;

	sprite->standing[1].y = 12;
	sprite->standing[1].x = 73;
	sprite->standing[1].h = 47;
	sprite->standing[1].w = 44;

	sprite->standing[2].y = 12;
	sprite->standing[2].x = 137;
	sprite->standing[2].h = 47;
	sprite->standing[2].w = 44;

	sprite->standing[3].y = 12;
	sprite->standing[3].x = 201;
	sprite->standing[3].h = 47;
	sprite->standing[3].w = 44;

	sprite->standing[4].y = 12;
	sprite->standing[4].x = 265;
	sprite->standing[4].h = 47;
	sprite->standing[4].w = 44;
	
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

	// have the player start at the lower left of the screen
	sprite->location.x = 100;
	sprite->location.y = SCREEN_HEIGHT - 200;
	sprite->location.h = sprite_height;
	sprite->location.w = sprite_width;
}
