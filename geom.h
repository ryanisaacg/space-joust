#pragma once

typedef struct {
	float x, y, width, height;
} Rectangle;

_Bool overlaps_rect(Rectangle a, Rectangle b);
