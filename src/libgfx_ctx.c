/*
 * libgfx - A simple graphics library built on top of OpenGL
 * Copyright (C) 2026 amritxyz
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/gfx.h"

typedef struct libgfx {
	GLFWwindow *window;
	int width;
	int height;
} libgfx;

static void glfw_error_callback(int error, const char *description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

libgfx
*gfx_init(int width, int height, const char *title)
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
	libgfx *ctx = malloc(sizeof(libgfx));
	if (!ctx) {
		fprintf(stderr, "Failed to allocate gfx context\n");
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
gfx_terminate(libgfx *ctx)
{
	if (!ctx) return;

	if (ctx->window) {
		glfwDestroyWindow(ctx->window);
	}

	glfwTerminate();
	free(ctx);
}

bool
gfx_is_running(libgfx *ctx)
{
	if (!ctx || !ctx->window) return false;
	return !glfwWindowShouldClose(ctx->window);
}

void
gfx_clear(libgfx *ctx, Color background)
{
	if (!ctx) return;

	glClearColor(background.r, background.g, background.b, background.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
gfx_present(libgfx *ctx)
{
	if (!ctx || !ctx->window) return;

	glfwSwapBuffers(ctx->window);
	glfwPollEvents();
}
