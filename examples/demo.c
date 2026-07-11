/* See LICENSE file for copyright and license details. */
#include "../include/libgfx.h"

int main(void)
{
	int height = 800;
	int width = 600;

	libgfx *ctx = gfx_init(width, width, "Libgfx demo");
	if (!ctx) return 1;

	Vec2 center = {
		(float)height/2,
		(float)width/2}
	;

	while (gfx_is_running(ctx)) {
		gfx_clear(ctx, BLACK);

		draw_poly(ctx, center, 4, 50, 45, TRANS, YELLOW, 2);

		gfx_present(ctx);
	}

	gfx_terminate(ctx);
	return 0;
}
