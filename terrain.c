#define COL_NONE 0
#define COL_LEFT 1
#define COL_RIGHT 2
#define COL_HEAD 3
#define COL_BOTTOM 4

typedef struct
{
	int platform_count;
	SDL_Rect platforms[20];

} terrain;

int terrain_on_the_ground(){
        return SCREEN_HEIGHT - 65 - 50;
}

int terrain_ground_level(){
        return SCREEN_HEIGHT - 65 - 50;
}

int terrain_in_the_air(){
	return 1;
}

terrain *terrain_init()
{
	terrain *level;
	level = (terrain*)malloc(sizeof(terrain));
	memset(level, 0, sizeof(terrain));

	level->platform_count - 20;

	level->platforms[0].x = 200;
	level->platforms[0].y = SCREEN_HEIGHT - 200;
	level->platforms[0].w = 400;
	level->platforms[0].h = 30;

	return level;
}

int terrain_draw(SDL_Renderer *renderer, terrain *level){
	SDL_SetRenderDrawColor(renderer, 0, 100, 0, 0);
	SDL_RenderFillRect(renderer, &level->platforms[0]);

	return 0;
}

// this returns an int specifying the collisson that Sonic has experienced
// COL_NONE - No collission
// COL_LEFT 
// COL_RIGHT 
// COL_HEAD 
// COL_BOTTOM 
int terrain_collission(player *sonic, terrain *level){
	int x;

	// if Sonic has crossed the top of a platform
	if((sonic->location.x + 44) > level->platforms[0].x && (sonic->location.x + 44 < (level->platforms[0].x + level->platforms[0].w))) 
	{
		if((sonic->location.y + 47 > level->platforms[0].y) && (sonic->location.y + 47 < level->platforms[0].y + level->platforms[0].h))
		{
			sonic->location.y = level->platforms[0].y - SONIC_HEIGHT;
			sonic->current_y_action = STANDING;
			printf("location.y %d\n: ", sonic->location.y);
			printf("level->platforms[0].y: %d\n", level->platforms[0].y);

			return COL_BOTTOM;
		}
	}

	return 0;
}

int terrain_free(terrain *level)
{
	free(level);
}
