#include "au.h"

#include <stdbool.h>
#include <stdio.h>

typedef struct {
	AU_Rectangle bounds;
	AU_Vector speed, acceleration;
	int charge_time, facing;
} Character;

void game_loop(AU_Engine* eng) {

	Character player = {{1, 1, 32, 32}, {0, 0}, {0, 0}, 0, 1};

	AU_Texture player_tex = au_load_texture(eng, "../player.png");

	AU_Tilemap map = au_tmap_init(800, 608, 32, 32);
	au_tmap_set(map, 128, 128, 5);

	const float max_speed = 3;
	const float accel = 0.5;
	const int charge_time = 30;

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
			player.facing = -1;
		}
		if (eng->current_keys[SDL_SCANCODE_D]) {
			player.acceleration.x += accel;
			player.facing = 1;
		}
		if (eng->current_keys[SDL_SCANCODE_SPACE] && player.charge_time <= 0) {
			player.charge_time = charge_time;
		}

		player.speed =  au_geom_vec_cmp_clamp(au_geom_vec_add(player.speed, player.acceleration), -max_speed, max_speed);

		if (player.charge_time > 0) {
			if (player.charge_time > charge_time / 2) {
				player.speed.x = 4 * max_speed * player.facing;
				player.charge_time--;
			} else {
				float progress = (float)player.charge_time / (5 * charge_time);
				printf("%f\n", progress);
				float bound = (1 - progress) * (player.speed.x - max_speed) + max_speed;
				au_util_clamp(player.speed.x, -bound, bound);
				player.charge_time--;
				if (player.charge_time == 0) {
					player.speed.x = 0;
				}
			}
		} else {
			if (au_geom_vec_len2(player.acceleration) == 0) {
				if (au_geom_vec_len2(player.speed) < 2.0f) {
					player.speed = (AU_Vector) {
						0, 0
					};
				} else {
					player.speed = au_geom_vec_scl(player.speed, 0.9f);
				}
			}
		}

		player.speed = au_tmap_slide(map, player.bounds, player.speed);
		player.bounds.x += player.speed.x;
		player.bounds.y += player.speed.y;

		au_draw_texture(eng, player_tex, player.bounds.x, player.bounds.y, player.bounds.width, player.bounds.height);

		for (int x = 0; x < 800; x += 32) {
			for (int y = 0; y < 600; y += 32) {
				if (au_tmap_get(map, x, y)) {
					au_draw_texture_rect(eng, player_tex, player.bounds);
				}
			}
		}
		au_end(eng);
	}
}
