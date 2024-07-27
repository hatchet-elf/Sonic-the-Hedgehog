#define COL_NONE 0
#define COL_LEFT 1
#define COL_RIGHT 2
#define COL_TOP 3
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
        return SCREEN_HEIGHT - 65 - 100;
}

int terrain_in_the_air(){
	return 1;
}

terrain *terrain_init()
{
	terrain *level;
	level = (terrain*)malloc(sizeof(terrain));
	memset(level, 0, sizeof(terrain));

	level->platform_count = 4;

	level->platforms[0].x = 200;
	level->platforms[0].y = SCREEN_HEIGHT - 200;
	level->platforms[0].w = 400;
	level->platforms[0].h = 30;

	level->platforms[1].x = 0;
	level->platforms[1].y = SCREEN_HEIGHT - 10;
	level->platforms[1].w = SCREEN_WIDTH;
	level->platforms[1].h = 30;

	level->platforms[2].x = 650;
	level->platforms[2].y = SCREEN_HEIGHT - 400;
	level->platforms[2].w = 400;
	level->platforms[2].h = 30;

	level->platforms[3].x = 650;
	level->platforms[3].y = SCREEN_HEIGHT - 50;
	level->platforms[3].w = 400;
	level->platforms[3].h = 30;



	return level;
}

int terrain_draw(SDL_Renderer *renderer, terrain *level){
	int x;

	for(x = 0; x < level->platform_count; x++)
	{
		SDL_SetRenderDrawColor(renderer, 0, 100, 0, 0);
		SDL_RenderFillRect(renderer, &level->platforms[x]);

	}

	return 0;
}

// this returns an int specifying side of a platform that Sonic has colided with
// COL_NONE - No collission
// COL_LEFT 
// COL_RIGHT 
// COL_TOP 
// COL_BOTTOM 
int terrain_collission(player *sonic, terrain *level)
{

	int x = 0;

	for(x = 0; x < level->platform_count; x++)
	{

		// Collission with the top of a platform
		if(sonic->location.x + 100 > level->platforms[x].x && (sonic->location.x < (level->platforms[x].x + level->platforms[x].w))) 
		{
			if((sonic->location.y + 100 > level->platforms[x].y) && (sonic->location.y + 100 < level->platforms[x].y + level->platforms[x].h))
			{
				sonic->location.y = level->platforms[x].y - SONIC_HEIGHT;
				sonic->current_y_action = STANDING;

				printf("COL_TOP\n");

				return COL_TOP;
			}
		}

		// Collission with the bottom of a platform
		if(sonic->location.x + 100 > level->platforms[x].x && (sonic->location.x < (level->platforms[x].x + level->platforms[x].w))) 
		{
			if((sonic->location.y > level->platforms[x].y) && (sonic->location.y < level->platforms[x].y + level->platforms[x].h))
			{
				sonic->location.y = level->platforms[x].y + level->platforms[x].h;
				sonic->current_y_action = FALLING;

				printf("COL_BOTTOM\n");

				return COL_BOTTOM;
			}
		}

		// Collission with the right hand side of a platform
		if(sonic->location.x < (level->platforms[x].x + level->platforms[x].w) && sonic->location.x > level->platforms[x].x) 
		{
			if((sonic->location.y + 100 > level->platforms[x].y) && (sonic->location.y < level->platforms[x].y + level->platforms[x].h))
			{
				sonic->location.x = level->platforms[x].x + level->platforms[x].w;
				sonic->current_y_action = FALLING;

				printf("COL_RIGHT\n");

				return COL_RIGHT;
			}
		}

		// Collission with the left hand side of a platform
		if(sonic->location.x + 100 > level->platforms[x].x && sonic->location.x + 100 < level->platforms[x].x + level->platforms[x].w) 
		{
			if((sonic->location.y + 100 > level->platforms[x].y) && (sonic->location.y + 100 < level->platforms[x].y + level->platforms[x].h))
			{
				sonic->location.x = level->platforms[x].x  - 100;
				sonic->current_y_action = FALLING;

				printf("COL_LEFT\n");

				return COL_LEFT;
			}
		}

		// No collission
		if(sonic->location.x + 44 < level->platforms[x].x || sonic->location.x > level->platforms[x].x + level->platforms[x].w)
		{
			if(sonic->location.y + 47 < level->platforms[x].y || sonic->location.y > level->platforms[x].y + level->platforms[x].y + level->platforms[x].h)
			{
				return COL_NONE;
			}
		}
	}
	return 2;
}

int terrain_free(terrain *level)
{
	free(level);
}
