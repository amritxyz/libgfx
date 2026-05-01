#include <stdio.h>
#include "../include/graphics.h"

int main(void)
{
	graphics_context_t *ctx = graphics_init(800, 600, "Graphics Library Demo");
	if (!ctx) {
		fprintf(stderr, "Failed to initialize graphics\n");
		return 1;
	}

	Color blue = color_create(0.1f, 0.3f, 0.8f, 1.0f);
	Color white = color_create(1.0f, 1.0f, 1.0f, 1.0f);

	while (graphics_is_running(ctx)) {
		graphics_clear(ctx, blue);
		graphics_present(ctx);
	}

	graphics_terminate(ctx);
	return 0;
}
