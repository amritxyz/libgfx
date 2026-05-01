#include "../include/graphics.h"

Color color_create(float r, float g, float b, float a)
{
	return (Color){r, g, b, a};
}

Vec2 vec2_create(float x, float y)
{
	return (Vec2){x, y};
}
