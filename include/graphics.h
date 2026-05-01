#pragma once

#include <stdbool.h>

typedef struct {
	float r, g, b, a;
} Color;

typedef struct {
	float x, y;
} Vec2;

typedef struct {
	float x, y, z;
} Vec3;

typedef struct graphics_context_t graphics_context_t;

graphics_context_t *graphics_init(int width, int height, const char *file);
void graphics_terminate(graphics_context_t *ctx);
bool graphics_is_running(graphics_context_t *ctx);
void graphics_clear(graphics_context_t *ctx, Color background);
void graphics_present(graphics_context_t *ctx);

/* Shaders */
void graphics_draw_triangle(graphics_context_t *ctx, Vec2 p1, Vec2 p2, Vec3 p3,
			    Color fill, Color border, float border_width);

void graphics_draw_rectangle(graphics_context_t *ctx, Vec2 pos, float width, float height,
			     Color fill, Color border, float border_width);

void graphics_draw_circle(graphics_context_t *ctx, Vec2 center, float radius,
			  Color fill, Color border, float border_width);

void graphics_draw_polygon(graphics_context_t *ctx, Vec2 *points, int point_count,
			  Color fill, Color border, float border_width);

Color color_create(float r, float g, float b, float a);
Vec2 vec2_create(float x, float y);
