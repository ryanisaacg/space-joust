#pragma once

typedef struct {
	float x, y;
} vec;

float vec_len(vec);
float vec_len2(vec);
vec vec_scale(vec, float);

typedef struct {
	float x, y, width, height;
} rect;

_Bool rect_overlaps(rect a, rect b);


typedef int tile;
typedef int* tilemap;
/*
 * tilemap: {tile_width, tile_height, width, height, ...}
 */
 
//Get the properties of the tilemap
#define TL_SIZE_REQUIRED(w, h) (4 + w * h)
#define TL_WIDTH(m) m[2]
#define TL_HEIGHT(m) m[3]
#define TL_TILE_WIDTH(m) m[0]
#define TL_TILE_HEIGHT(m) m[1]

//Get the value at the index of the tilemap
tile tl_get(tilemap m, int x, int y);
//Set the value at the index of the tilemap
void tl_set(tilemap m, int x, int y, tile val);
//Get the first value found within the square
tile tl_first(tilemap m, float x, float y, float w, float h);
tile tl_first_rect(tilemap, rect);
//Get the max amount a rect can move with a velocity
vec tl_slide(tilemap, rect, vec);
