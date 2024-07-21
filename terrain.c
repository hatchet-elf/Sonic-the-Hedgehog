#define COL_NONE 0
#define COL_LEFT 1
#define COL_RIGHT 2
#define COL_HEAD 3
#define COL_BOTTOM 4

int terrain_on_the_ground(){
        return SCREEN_HEIGHT - 65 - 50;
}

int terrain_ground_level(){
        return SCREEN_HEIGHT - 65 - 50;
}

int terrain_in_the_air(){
	return 1;
}

int terrain_init(SDL_Rect platforms[20]){
	platforms[0].x = 200;
	platforms[0].y = SCREEN_HEIGHT - 200;
	platforms[0].w = 400;
	platforms[0].h = 30;

	return 0;
}

int terrain_draw(SDL_Renderer *renderer, SDL_Rect *platforms){
	SDL_SetRenderDrawColor(renderer, 0, 100, 0, 0);
	SDL_RenderFillRect(renderer, platforms);

	return 0;
}

// this returns an int specifying the collisson
// COL_NONE - No collission
// COL_LEFT 
// COL_RIGHT 
// COL_HEAD 
// COL_BOTTOM 
int terrain_collission_with_platform(SDL_Rect platforms[0]){
	return 0;
}
