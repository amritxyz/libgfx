#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>
#include <GLFW/glfw3.h>

#include "../include/graphics.h"

typedef struct graphics_context_t {
	GLFWwindow *window;
	int width;
	int height;
} graphics_context_t;

static void glfw_error_callback(int error, const char *description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

graphics_context_t
*graphics_init(int width, int height, const char *title)
{
	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _MACOS
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create window */
	GLFWwindow *window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return NULL;
	}

	/* Make context current */
	glfwMakeContextCurrent(window);

	/* Enable vsync (60 FPS cap) */
	glfwSwapInterval(1);

	/* Allocate context struct */
	graphics_context_t *ctx = malloc(sizeof(graphics_context_t));
	if (!ctx) {
		fprintf(stderr, "Failed to allocate graphics context\n");
		glfwDestroyWindow(window);
		glfwTerminate();
		return NULL;
	}

	ctx->window = window;
	ctx->width = width;
	ctx->height = height;

	/* Set up OpenGL viewport */
	glViewport(0, 0, width, height);

	/* Enable blending for transparency */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return ctx;
}

void
graphics_terminate(graphics_context_t *ctx)
{
	if (!ctx) return;

	if (ctx->window) {
		glfwDestroyWindow(ctx->window);
	}

	glfwTerminate();
	free(ctx);
}

bool
graphics_is_running(graphics_context_t *ctx)
{
	if (!ctx || !ctx->window) return false;
	return !glfwWindowShouldClose(ctx->window);
}

void
graphics_clear(graphics_context_t *ctx, Color background)
{
	if (!ctx) return;

	glClearColor(background.r, background.g, background.b, background.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
graphics_present(graphics_context_t *ctx)
{
	if (!ctx || !ctx->window) return;

	glfwSwapBuffers(ctx->window);
	glfwPollEvents();
}
