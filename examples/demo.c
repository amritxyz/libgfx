/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include "../include/gfx.h"

int main(void)
{
	libgfx *ctx = gfx_init(800, 600, "Libgfx demo");
	if (!ctx) {
		fprintf(stderr, "Failed to initialize graphics\n");
		return 1;
	}

	while (gfx_is_running(ctx)) {
		gfx_clear(ctx, ORANGE);
		gfx_present(ctx);
	}

	gfx_terminate(ctx);
	return 0;
}
