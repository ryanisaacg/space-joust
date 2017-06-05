#include "au.h"

#include <stdbool.h>

typedef struct {
	AU_Rectangle bounds;
	AU_Vector speed, acceleration;
} Character;

void game_loop(AU_Engine* eng) {

	Character player = {{0, 0, 32, 32}, {0, 0}, {0, 0}};

	AU_Texture player_tex = au_load_texture(eng, "../player.png");

	while (eng->should_continue) {
		au_begin(eng);

		player.acceleration = (AU_Vector) {
			0, 0
		};
		//Keyboard controls
		if (eng->current_keys[SDL_SCANCODE_W]) {
			player.acceleration.y -= 0.1;
		}
		if (eng->current_keys[SDL_SCANCODE_S]) {
			player.acceleration.y += 0.1;
		}
		if (eng->current_keys[SDL_SCANCODE_A]) {
			player.acceleration.x -= 0.1;
		}
		if (eng->current_keys[SDL_SCANCODE_D]) {
			player.acceleration.x += 0.1;
		}

		player.speed = au_geom_vec_cmp_clamp(au_geom_vec_add(player.speed, player.acceleration), -8, 8);
		if (au_geom_vec_len2(player.acceleration) == 0) {
			if (au_geom_vec_len2(player.speed) < 2.0f) {
				player.speed = (AU_Vector) {
					0, 0
				};
			} else {
				player.speed = au_geom_vec_scl(player.speed, 0.9f);
			}
		}
		player.bounds.x += player.speed.x;
		player.bounds.y += player.speed.y;

		au_draw_texture(eng, player_tex, player.bounds.x, player.bounds.y, player.bounds.width, player.bounds.height);

		au_end(eng);
	}
}
