#include "au.h"

typedef struct {
	AU_Rectangle bounds;
	AU_Vector speed, acceleration;
} Character;

void game_loop(AU_Engine* eng) {

	Character player;

	while(eng->should_continue) {
		au_begin(eng);

		player.acceleration = (AU_Vector) { 0, 0 };
		//Keyboard controls
		if(eng->current_keys[SDL_SCANCODE_W]) player.acceleration.y -= 0.1;
		if(eng->current_keys[SDL_SCANCODE_S]) player.acceleration.y += 0.1;
		if(eng->current_keys[SDL_SCANCODE_A]) player.acceleration.x -= 0.1;
		if(eng->current_keys[SDL_SCANCODE_D]) player.acceleration.x += 0.1;

		player.speed = au_geom_vec_add(player.speed, player.acceleration);
		player.bounds.x += player.speed.x;
		player.bounds.y += player.speed.y;

		au_end(eng);
	}
}
