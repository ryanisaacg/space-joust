#include "au.h"

#include <stdbool.h>
#include <stdio.h>

typedef struct {
	AU_Rectangle bounds;
	AU_Vector speed, acceleration;
	int charge_time, facing;
} Character;

const int charge_time = 30;

#define INT_STRING_LENGTH ((8 * sizeof(int) - 1) / 3 + 2)

static Character apply_controls(AU_Engine* eng, Character player, AU_Tilemap map,
		int left, int right, int up, int down, int charge, AU_ParticleEmitter *emitter) {

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
		emitter->velocity_min.x = abs(emitter->velocity_min.x) * -player.facing;
		emitter->velocity_max.x = abs(emitter->velocity_max.x) * -player.facing;
		emitter->top_left = (AU_Vector) { player.bounds.x, player.bounds.y };
		emitter->bottom_right = (AU_Vector) { player.bounds.x + player.bounds.width, player.bounds.y + player.bounds.height };
		au_add_particles(eng, emitter);
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

void draw_player(AU_Engine* eng, Character player, AU_TextureRegion region) {
	AU_Sprite sprite = au_sprite_new(region);
	sprite.transform.x = player.bounds.x;
	sprite.transform.y = player.bounds.y;
	sprite.transform.flip_x = player.facing == -1;
	au_draw_sprite(eng, &sprite);
}


void game_loop(AU_Engine* eng) {

	Character player1 = {{1, 1, 32, 32}, {0, 0}, {0, 0}, 0, 1};
	Character player2 = {{500, 500, 32, 32}, {0, 0}, {0, 0}, 0, 1};

	int score1 = 0;
	int score2 = 0;
	int restart_timer = -1;

	char buffer[INT_STRING_LENGTH];
	//Length to store PLAYER N WINS (plus a nulll terminator)
	char win_buffer[14];

	AU_Texture player1_tex = au_load_texture(eng, "../player-blue.png");
	AU_Texture player2_tex = au_load_texture(eng, "../player-red.png");
	AU_Texture block_tex = au_load_texture(eng, "../block.png");
	AU_Texture particle_tex = au_load_texture(eng, "../particle.png");
	//Create the particle emitter
	AU_ParticleEmitter emitter = au_particle_emitter_new((AU_TextureRegion[]) { au_tex_region(particle_tex) }, 1);
	emitter.velocity_min = (AU_Vector) { 2, 0 };
	emitter.velocity_max = (AU_Vector) { 8, 0 };
	emitter.lifetime_min = 1;
	emitter.lifetime_max = 15;
	emitter.particle_min = 15;
	emitter.particle_max = 30;

	AU_Font* score_font = au_load_font(eng, 18, AU_WHITE, "../DejaVuSans.ttf");
	AU_Font* result_font = au_load_font(eng, 36, AU_WHITE, "../DejaVuSans.ttf");
	AU_Tilemap map = au_tmap_init(800, 608, 32, 32);
	au_tmap_set(map, 128, 128, 5);

	while (eng->should_continue) {
		au_begin(eng, AU_CYAN);
		if(restart_timer > 0) {
			restart_timer--;
			if(restart_timer == 0) {
				player1 = (Character) {{1, 1, 32, 32}, {0, 0}, {0, 0}, 0, 1};
				player2 = (Character) {{500, 500, 32, 32}, {0, 0}, {0, 0}, 0, 1};
			}
		} else {
			player1 = apply_controls(eng, player1, map,
					SDL_SCANCODE_A, SDL_SCANCODE_D, SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_SPACE, &emitter);
			player2 = apply_controls(eng, player2, map, SDL_SCANCODE_LEFT, 
					SDL_SCANCODE_RIGHT, SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_PERIOD, &emitter);
			if(au_geom_rect_overlaps_rect(player1.bounds, player2.bounds)) {
				bool charging1 = player1.charge_time > charge_time / 2;
				bool charging2 = player2.charge_time > charge_time / 2;
				if(charging1 && charging2) {
					if(player1.charge_time > player2.charge_time) {
						score1++;
						strcpy(win_buffer, "Player 1 wins");
						charging2 = false;
					} else if(player1.charge_time < player2.charge_time) {
						score2++;
						strcpy(win_buffer, "Player 2 wins");
						charging1 = false;
					} else {
						player1.speed.x = -10 * player1.facing;
						player2.speed.x = -10 * player2.facing;
					}
				} else if(charging1) {
					score1++;
					strcpy(win_buffer, "Player 1 wins");
				} else if(charging2) {
					score2++;
					strcpy(win_buffer, "Player 2 wins");
				}
				//Check if the round should restart
				if(charging1 != charging2) {
					restart_timer = 60;
				}
			}
		}
		//Draw code
		draw_player(eng, player1, au_tex_region(player1_tex));
		draw_player(eng, player2, au_tex_region(player2_tex));

		AU_TextureRegion block_region = au_tex_region(block_tex);
		for (int x = 0; x < 800; x += 32) {
			for (int y = 0; y < 600; y += 32) {
				if (au_tmap_get(map, x, y)) {
					au_draw_texture(eng, block_region, x, y);
				}
			}
		}

		if(restart_timer > 0) {
			au_draw_string(eng, result_font, win_buffer, 300, 64);
		}

		sprintf(buffer, "%d", score1);
		au_draw_string(eng, score_font, buffer, 32, 16);
		sprintf(buffer, "%d", score2);
		au_draw_string(eng, score_font, buffer, 600, 16);

		au_end(eng);
	}
}
