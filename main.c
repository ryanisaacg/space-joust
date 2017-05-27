#include <SDL_gpu.h>

#include "game.h"

#undef main
int main() {
	GPU_Target *screen = GPU_Init(800, 600, GPU_DEFAULT_INIT_FLAGS);
	if(!screen)
		return -1;

	game_loop(screen);

	GPU_Quit();
	return 0;
}
