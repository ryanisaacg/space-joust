#include "au.h"

#include <stdbool.h>
#include <stdio.h>

typedef struct {
	AU_Rectangle bounds;
	AU_Vector speed, acceleration;
	int charge_time, facing;
} Character;

const int charge_time = 30;

#define INT_STRING_LENGTH ((CHAR_BIT * sizeof(int) - 1) / 3 + 2)

static Character apply_controls(AU_Engine* eng, Character player, AU_Tilemap map,
		int left, int right, int up, int down, int charge) {

	const float max_speed = 3;
	const float accel = 0.5;

	player.acceleration = (AU_Vector) {
		0, 0
	};
	//Keyboard controls
	if (eng->current_keys[up]) {
		player.acceleration.y -= accel;
	}
	if (eng->current_keys[down]) {
		player.acceleration.y += accel;
	}
	if (eng->current_keys[left]) {
		player.acceleration.x -= accel;
		player.facing = -1;
	}
	if (eng->current_keys[right]) {
		player.acceleration.x += accel;
		player.facing = 1;
	}
	if (eng->current_keys[charge] && player.charge_time <= 0) {
		player.charge_time = charge_time;
	}

	//Movement code
	player.speed =  au_geom_vec_cmp_clamp(au_geom_vec_add(player.speed, player.acceleration), -max_speed, max_speed);

	if (player.charge_time > 0) {
		if (player.charge_time > charge_time / 2) {
			player.speed.x = 4 * max_speed * player.facing;
			player.charge_time--;
		} else {
			float progress = (float)player.charge_time / (5 * charge_time);
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

	return player;
}

void game_loop(AU_Engine* eng) {

	Character player1 = {{1, 1, 32, 32}, {0, 0}, {0, 0}, 0, 1};
	Character player2 = {{500, 500, 32, 32}, {0, 0}, {0, 0}, 0, 1};

	int score1 = 0;
	int score2 = 0;
	int restart_timer = -1;

	char buffer[INT_STRING_LENGTH];

	AU_Texture player_tex = au_load_texture(eng, "../player.png");
	AU_Font* font = au_load_font(eng, 18, (AU_Color) { 1, 1, 1, 1 }, "../DejaVuSans.ttf");
	AU_Tilemap map = au_tmap_init(800, 608, 32, 32);
	au_tmap_set(map, 128, 128, 5);

	while (eng->should_continue) {
		au_begin(eng);
		if(restart_timer > 0) {
			restart_timer--;
			if(restart_timer == 0) {
				player1 = (Character) {{1, 1, 32, 32}, {0, 0}, {0, 0}, 0, 1};
				player2 = (Character) {{500, 500, 32, 32}, {0, 0}, {0, 0}, 0, 1};
			}
		} else {
			player1 = apply_controls(eng, player1, map,
					SDL_SCANCODE_A, SDL_SCANCODE_D, SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_SPACE);
			player2 = apply_controls(eng, player2, map, SDL_SCANCODE_LEFT, 
					SDL_SCANCODE_RIGHT, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_PERIOD);
			if(au_geom_rect_overlaps_rect(player1.bounds, player2.bounds)) {
				bool charging1 = player1.charge_time > charge_time / 2;
				bool charging2 = player2.charge_time > charge_time / 2;
				if(charging1 && charging2) {
					//TODO: have the players bounce off each other
				} else if(charging1) {
					score1++;
				} else if(charging2) {
					score2++;
				}
				//Check if the round should restart
				if(charging1 != charging2) {
					restart_timer = 60;
				}
			}
		}
		//Draw code
		AU_TextureRegion region = au_tex_region(player_tex);
		au_draw_texture_rect(eng, region, player1.bounds);
		au_draw_texture_rect(eng, region, player2.bounds);

		for (int x = 0; x < 800; x += 32) {
			for (int y = 0; y < 600; y += 32) {
				if (au_tmap_get(map, x, y)) {
					au_draw_texture(eng, region, x, y);
				}
			}
		}

		sprintf(buffer, "%d", score1);
		au_draw_string(eng, font, buffer, 32, 0);
		sprintf(buffer, "%d", score2);
		au_draw_string(eng, font, buffer, 600, 0);

		au_end(eng);
	}
}
