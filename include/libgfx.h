/* See LICENSE file for copyright and license details. */
#pragma once

#include <stdbool.h>

#define COLOR_TO_FLOAT(c) { \
	(c).r / 255.0f, \
	(c).g / 255.0f, \
	(c).b / 255.0f, \
	(c).a / 255.0f, \
}

#define BLACK	(Color){ 0, 0, 0, 255 }
#define BLANK	(Color){ 0, 0, 0, 0 }
#define BLUE	(Color){ 0, 0, 255, 255 }
#define BROWN	(Color){ 165, 42, 42, 255 }
#define CYAN	(Color){ 0, 255, 255, 255 }
#define GRAY	(Color){ 128, 128, 128, 255 }
#define GREEN	(Color){ 0, 128, 0, 255 }
#define LIME	(Color){ 0, 255, 0, 255 }
#define MAGENTA	(Color){ 255, 0, 255, 255 }
#define ORANGE	(Color){ 255, 165, 0, 255 }
#define PINK	(Color){ 255, 192, 203, 255 }
#define PURPLE	(Color){ 128, 0, 128, 255 }
#define RED	(Color){ 255, 0, 0, 255 }
#define SKYBLUE	(Color){ 100, 180, 220, 255 }
#define VIOLET	(Color){ 238, 130, 238, 255 }
#define WHITE	(Color){ 255, 255, 255, 255 }
#define YELLOW	(Color){ 255, 255, 0, 255 }

typedef struct {
	float x;
	float y;
} Vec2;

typedef struct {
	float x;
	float y;
	float z;
} Vec3;

typedef struct {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} Color;

typedef struct {
	float x;
	float y;
	float width;
	float height;
} Rect;

typedef struct {
	float m0, m4, m8,  m12;
	float m1, m5, m9,  m13;
	float m2, m6, m10, m14;
	float m3, m7, m11, m15;
} Mat4;

typedef struct libgfx libgfx;

libgfx *gfx_init(int width, int height, const char *file);
void gfx_terminate(libgfx *ctx);
bool gfx_is_running(libgfx *ctx);
void gfx_clear(libgfx *ctx, Color background);
void gfx_present(libgfx *ctx);

/* Shaders */
void draw_triangle(libgfx *ctx, Vec2 p1, Vec2 p2, Vec3 p3,
			    Color fill, Color border, float border_width);

void draw_rectangle(libgfx *ctx, Vec2 pos, float width, float height,
			     Color fill, Color border, float border_width);

void draw_circle(libgfx *ctx, Vec2 center, float radius,
			  Color fill, Color border, float border_width);

void draw_polygon(libgfx *ctx, Vec2 *points, int point_count,
			  Color fill, Color border, float border_width);

Color color_create(float r, float g, float b, float a);
Vec2 vec2_create(float x, float y);
