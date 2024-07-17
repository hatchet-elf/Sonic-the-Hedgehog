all:
	gcc -o sonic ./sonic.c -lSDL2 -ldl -lm -lSDL2_image
	 

debug:
	gcc -o sonic ./sonic.c -g -lSDL2 -ldl -lm -lSDL2_image
