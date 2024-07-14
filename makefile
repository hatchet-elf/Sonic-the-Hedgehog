all:
	gcc -o spriteanimation ./spriteanimation.c -lSDL2 -ldl -lm -lSDL2_image
	 

debug:
	gcc -o spriteanimation ./spriteanimation.c -g -lSDL2 -ldl -lm -lSDL2_image
