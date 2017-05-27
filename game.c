#include "game.h"

#include <stdlib.h>

#include "geom.h"

void game_loop(GPU_Target *target) {
	Uint8 done;
	SDL_Event event;
	
	tilemap m = tl_init(640, 480, 32, 32, malloc(sizeof(*m) * TL_SIZE_REQUIRED(640, 480)));
	
	done = 0;
	while(!done) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) done = 1;
			else if(event.type == SDL_KEYDOWN)
			{
				if(event.key.keysym.sym == SDLK_ESCAPE)
				done = 1;
			}
		}
		GPU_Clear(target);

		GPU_Flip(target);
	}

	free(m);
}
