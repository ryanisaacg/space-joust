#pragma once

typedef struct {
	float x, y, width, height;
} rect;

_Bool overlaps_rect(rect a, rect b);


typedef int tile;
typedef struct {
	int tile_width, tile_height, width, height;
	tile* buffer;
} tilemap;

//Get the value at the index of the tilemap
tile tl_get(tilemap m, int x, int y);
//Set the value at the index of the tilemap
void tl_set(tilemap m, int x, int y, tile val);
//Get the first value found within the square
tile tl_first(tilemap m, int x, int y, int w, int h);
tile tl_first_rect(tilemap, rect);
