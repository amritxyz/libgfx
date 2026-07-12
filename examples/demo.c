/* See LICENSE file for copyright and license details. */
#include "../include/libgfx.h"

struct Obj {
	Vec2	position;
	int	sides;
	float	radius;
	float	rotation;
	Color	fill;
	Color	border;
	float	border_width;
};

static void
change_obj_position(libgfx *ctx, struct Obj *obj)
{
	if (gfx_is_key_down(ctx, GLFW_KEY_H))
		obj->position.x -= 3.0f;
	if (gfx_is_key_down(ctx, GLFW_KEY_J))
		obj->position.y += 3.0f;
	if (gfx_is_key_down(ctx, GLFW_KEY_K))
		obj->position.y -= 3.0f;
	if (gfx_is_key_down(ctx, GLFW_KEY_L))
		obj->position.x += 3.0f;
}

int main(void)
{
	int height = 400;
	int width = 900;

	libgfx *ctx = gfx_init(width, height, "float_win");
	if (!ctx) return 1;

	struct Obj obj = {
		.position = {(float)width / 2.0f, (float)height / 2.0f},
		.sides = 4,
		.radius = 50.0f,
		.rotation = 45.0f,
		.fill = TRANS,
		.border = YELLOW,
		.border_width = 2.0f,
	};

	while (gfx_is_running(ctx)) {
		if (gfx_is_key_pressed(ctx, GLFW_KEY_Q))
			gfx_request_close(ctx);

		change_obj_position(ctx, &obj);

		gfx_clear(ctx, BLACK);

		draw_poly(ctx, obj.position, obj.sides, obj.radius,
			  obj.rotation, obj.fill, obj.border, obj.border_width);

		gfx_present(ctx);
	}

	gfx_terminate(ctx);
	return 0;
}
