#include "au.h"
#include "game.h"

#undef main
int main() {
	AU_Engine* eng = au_init("Space Joust", 800, 600);

	game_loop(eng);

	au_quit(eng);
	return 0;
}
