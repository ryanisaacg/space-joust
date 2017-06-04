#include "au.h"

void game_loop(AU_Engine* eng) {
	while(eng->should_continue) {
		au_begin(eng);

		au_end(eng);
	}
}
