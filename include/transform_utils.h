#pragma once

#include "geometry.h"
#include <SDL2/SDL.h>

struct Transform {
	double scale;
	double offsetX;
	double offsetY;
	int screenH; // Need for Y-flip
};

SDL_Point worldToScreen(double x, double y, const Transform& t);
Transform makeTransform(const BoundingBox& box, int screenW, int screenH, double padding = 20.0);
