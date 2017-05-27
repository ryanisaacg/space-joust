#include "geom.h"

#include <math.h>

float vec_len(vec v) {
	return sqrt(vec_len2(v));
}

float vec_len2(vec v) {
	return v.x * v.x + v.y * v.y;
}

vec vec_scale(vec v, float s) {
	return {v.x * s, v.y * s};
}

_Bool rect_overlaps(rect a, rect b) {
	return a.x < b.x + b.width && a.x + a.width > b.x && a.y < b.y + b.height && a.y + a.height > b.y;
}

#define VALID (x >= 0 && y >= 0 && x < TL_WIDTH(map) && y < TL_HEIGHT(map))

inline static int tl_index(tilemap map, int x, int y) {
	x /= TL_TILE_WIDTH(map);
	y /= TL_TILE_HEIGHT(map);
	return x * TL_TILE_HEIGHT(map) + y + 4;
}

tile tl_get (tilemap map, int x, int y) {
	if(VALID) return map[tl_index(map, x, y)];
	else return -1;
}

void tl_set (tilemap map, int x, int y, tile val) {
	if(VALID) map[tl_index(map, x, y)] = val;
}

tile tl_first (tilemap map, int x, int y, int w, int h) {
	int left = x / TL_TILE_WIDTH(map);
	int top = y / TL_TILE_HEIGHT(map);
	int right = (x + w) / TL_TILE_WIDTH(map);
	int bottom = (y + h) / TL_TILE_HEIGHT(map);
	if(left < 0 || top < 0 || right > TL_WIDTH(map) / TL_TILE_WIDTH(map) || bottom > TL_HEIGHT(map) / TL_TILE_HEIGHT(map))
		return -1;
	for(int i = left; i <= right; i++) {
		for(int j = top; j <= bottom; j++) {
			int index = i * TL_TILE_HEIGHT(map) + j + 4;
			if(map[index])
				return map[index];
		}
	}
	return 0;
}

tile tl_first_rect(tilemap m, rect r) {
	return tl_first(m, (int)r.x, (int)r.y, (int)r.width, (int)r.height);
}
