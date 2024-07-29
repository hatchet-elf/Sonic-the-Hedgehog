#define COL_NONE 0
#define COL_LEFT 1
#define COL_RIGHT 2
#define COL_TOP 3
#define COL_BOTTOM 4

typedef struct
{
	int platform_count;
	SDL_Rect platforms[20];

} map;

int map_on_the_ground(){
        return SCREEN_HEIGHT - 65 - 50;
}

int map_ground_level(){
        return SCREEN_HEIGHT - 65 - 100;
}

int map_in_the_air(){
	return 1;
}

map *map_init()
{
	map *level;
	level = (map*)malloc(sizeof(map));
	memset(level, 0, sizeof(map));

	level->platform_count = 4;

	level->platforms[0].x = 200;
	level->platforms[0].y = SCREEN_HEIGHT - 200;
	level->platforms[0].w = 400;
	level->platforms[0].h = 30;

	level->platforms[1].x = 0;
	level->platforms[1].y = SCREEN_HEIGHT - 10;
	level->platforms[1].w = SCREEN_WIDTH;
	level->platforms[1].h = 30;

	level->platforms[2].x = 625;
	level->platforms[2].y = SCREEN_HEIGHT - 390;
	level->platforms[2].w = 400;
	level->platforms[2].h = 30;

	level->platforms[3].x = 850;
	level->platforms[3].y = SCREEN_HEIGHT - 590;
	level->platforms[3].w = 400;
	level->platforms[3].h = 30;

	return level;
}

int map_draw(SDL_Renderer *renderer, map *level){
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
int map_collission(SDL_Rect *player, map *level)
{
	// make a variable for each member of the structures
	// the reason is that the code will be quicker rather than derefrencing pointers repeateadly
	// also the code is far easier to read
	const int player_x = player->x;
	const int player_y = player->y;
	const int player_h = player->h;
	const int player_w = player->w;

	int level_x;
	int level_y;
	int level_h;
	int level_w;

	int x = 0;



	for(x = 0; x < level->platform_count; x++)
	{
		level_x = level->platforms[x].x;
		level_y = level->platforms[x].y;
		level_h = level->platforms[x].h;
		level_w = level->platforms[x].w;

		// Collission with the top of a platform
		if((player_y + player_h > level_y) && (player_y + player_h < level_y + level_h))
		{
			if((player->x + player_h > level_x && player_x + player_h < level_x + level_w) ||
					(player_x > level_x && player_x < level_x + level_w))
			{
				player->y = level_y - player_h;

				//printf("platform: %d  COL_TOP\n", x);

				return COL_TOP;
			}
		}

		// Collission with the bottom of a platform
		if(player_y < level_y + level_h && player_y > level_y )
		{
			if((player_x + player_h > level_x && player_x + player_h < level_x + level_w) ||
					(player_x > level_x && player_x < level_x + level_w))
			{
				player->y = level_y + level_h;

				printf("platform: %d  COL_BOTTOM\n", x);

				return COL_BOTTOM;
			}
		}

		// Collission with the right hand side of a platform
		if(player_x < level_x + level_w && player_x > level_x) 
		{
			if(player_y > level_y && player_y < level_y + level_h ||
					player_y + player_h > level_y && player_y + player_h < level_y + level_h ||
					// this is because Sonic can be taller than the platform
					// so a condition where the top of sonic is above the platform and the bottom of Sonic
					// is below the platform will be caught
					player_y < level_y && player_y + player_h > level_y + level_h)
			{
				player->x = level_x + level_w;

				return COL_RIGHT;
			}
		}

		// Collission with the left hand side of a platform
		if(player_x + player_w > level_x && player_x + player_w < level_x + level_w) 
		{
			if(player_y > level_y && player_y < level_y + level_h ||
					player_y + player_h > level_y && player_y + player_h < level_y + level_h ||
					player_y < level_y && player_y + player_h > level_y + level_h)
			{
				player->x = level_x  - player_w;


				return COL_LEFT;
			}
		}
	}
	return COL_NONE;
}

int map_free(map *level)
{
	free(level);
}
