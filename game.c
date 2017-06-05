#include "au.h"

#include <stdbool.h>
#include <stdio.h>

typedef struct {
	AU_Rectangle bounds;
	AU_Vector speed, acceleration;
	int charge_time;
} Character;

void game_loop(AU_Engine* eng) {

	Character player = {{1, 1, 32, 32}, {0, 0}, {0, 0}, 0};

	AU_Texture player_tex = au_load_texture(eng, "../player.png");

	AU_Tilemap map = au_tmap_init(800, 608, 32, 32);
	au_tmap_set(map, 128, 128, 5);

	const float max_speed = 3;
	const float accel = 0.5;

	while (eng->should_continue) {
		au_begin(eng);

		player.acceleration = (AU_Vector) {
			0, 0
		};
		//Keyboard controls
		if (eng->current_keys[SDL_SCANCODE_W]) {
			player.acceleration.y -= accel;
		}
		if (eng->current_keys[SDL_SCANCODE_S]) {
			player.acceleration.y += accel;
		}
		if (eng->current_keys[SDL_SCANCODE_A]) {
			player.acceleration.x -= accel;
		}
		if (eng->current_keys[SDL_SCANCODE_D]) {
			player.acceleration.x += accel;
		}

		player.speed = au_tmap_slide(map, player.bounds,
									 au_geom_vec_cmp_clamp(
									 au_geom_vec_add(player.speed,
									 player.acceleration), -max_speed, max_speed));

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

		for(int x = 0; x < 800; x += 32) {
			for(int y = 0; y < 600; y += 32) {
				if(au_tmap_get(map, x, y)) {
					au_draw_texture(eng, player_tex, x, y, 32, 32);
				}
			}
		}
		au_end(eng);
	}
}
