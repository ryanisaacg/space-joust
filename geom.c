#include "geom.h"

_Bool overlaps_rect(rect a, rect b) {
	return a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y;
}

#define VALID (x >= 0 && y >= 0 && x < map.width && y < map.height)

inline static int tl_index(tilemap map, int x, int y) {
	x /= map.tile_width;
	y /= map.tile_height;
	return x * map.tile_height + y;
}

tile tl_get (tilemap map, int x, int y) {
	if(VALID) return map.buffer[tl_index(map, x, y)];
	else return -1;
}

void tl_set (tilemap map, int x, int y, tile val) {
	if(VALID) map.buffer[tl_index(map, x, y)] = val;
}

tile tl_first (tilemap map, int x, int y, int w, int h) {
	int left = x / map.tile_width;
	int top = y / map.tile_height;
	int right = (x + w) / map.tile_width;
	int bottom = (y + h) / map.tile_height;
	if(left < 0 || top < 0 || right > map.width / map.tile_width || bottom > map.height / map.tile_height)
		return -1;
	for(int i = left; i <= right; i++)
		for(int j = top; j <= bottom; j++)
			if(map.buffer[i * map.tile_height + j])
				return map.buffer[i * map.tile_height + j];
	return 0;
}

tile tl_first_rect(tilemap m, rect r) {
	return tl_first(m, (int)r.x, (int)r.y, (int)r.width, (int)r.height);
}
