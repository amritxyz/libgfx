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

#include "../glad/include/glad/gl.h"
#include <GL/glext.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/libgfx.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif /* M_PI */

typedef struct libgfx {
	GLFWwindow *window;
	int width;
	int height;

	/* OpenGL objects */
	GLuint shader;
	GLuint vao;
	GLuint vbo;
} libgfx;

static const char *vert_src =
	"#version 330 core\n"
	"layout (location = 0) in vec2 aPos;\n"
	"layout (location = 1) in vec4 aColor;\n"
	"out vec4 vColor;\n"
	"uniform mat4 projection;\n"
	"void main() {\n"
	"	gl_Position = projection * vec4(aPos, 0.0, 1.0);\n"
	"	vColor = aColor;\n"
	"}\n";

static const char *frag_src =
	"#version 330 core\n"
	"in vec4 vColor;\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"	FragColor = vColor;\n"
	"}\n";

static void
glfw_error_callback(int error, const char *description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static GLuint
compile_shader(GLenum type, const char *source)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char info_log[512];
		glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
		fprintf(stderr, "Shader compilation failed: %s\n", info_log);
		glDeleteShader(shader);
		return 0;
	}
	return shader;
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

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create window */
	GLFWwindow *window = glfwCreateWindow(width, height, title, NULL,
			     NULL);
	if (!window) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return NULL;
	}

	/* Make context current */
	glfwMakeContextCurrent(window);

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		fprintf(stderr, "Failed to initialize GLAD\n");
		glfwDestroyWindow(window);
		glfwTerminate();
		return NULL;
	}

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

	GLuint vert = compile_shader(GL_VERTEX_SHADER, vert_src);
	GLuint frag = compile_shader(GL_FRAGMENT_SHADER, frag_src);
	if (!vert || !frag) {
		free(ctx);
		glfwDestroyWindow(window);
		glfwTerminate();
		return NULL;
	}

	ctx->shader = glCreateProgram();
	glAttachShader(ctx->shader, vert);
	glAttachShader(ctx->shader, frag);
	glLinkProgram(ctx->shader);
	glDeleteShader(vert);
	glDeleteShader(frag);

	/* setup orthographic projection */
	float left = 0.0f, right = (float)width;
	float bottom = (float)height, top = 0.0f;
	float near = -1.0f, far = 1.0f;

	float ortho[16] = {0};
	ortho[0] = 2.0f / (right - left);
	ortho[5] = 2.0f / (top - bottom);
	ortho[10] = -2.0f / (far - near);
	ortho[12] = -(right + left) / (right - left);
	ortho[13] = -(top + bottom) / (top - bottom);
	ortho[14] = -(far + near) / (far - near);
	ortho[15] = 1.0f;

	glUseProgram(ctx->shader);
	glUniformMatrix4fv(glGetUniformLocation(ctx->shader, "projection"), 1,
			   GL_FALSE, ortho);

	/* setup vao and vbo */
	glGenVertexArrays(1, &ctx->vao);
	glGenBuffers(1, &ctx->vbo);

	glBindVertexArray(ctx->vao);
	glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
			      (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
			      (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	/* Enable blending for transparency */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return ctx;
}

void
gfx_terminate(libgfx *ctx)
{
	if (!ctx) return;

	glDeleteVertexArrays(1, &ctx->vao);
	glDeleteBuffers(1, &ctx->vbo);
	glDeleteProgram(ctx->shader);

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

	glClearColor(background.r / 255.0f, background.g / 255.0f,
		     background.b / 255.0f, background.a / 255.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
gfx_present(libgfx *ctx)
{
	if (!ctx || !ctx->window) return;

	glfwSwapBuffers(ctx->window);
	glfwPollEvents();
}

void
draw_pixel(libgfx *ctx, int x, int y, Color color)
{
	if (!ctx) return;
	float vertices[6] = {
		(float)x, (float)y,
		color.r / 255.0f, color.g / 255.0f,
		color.b / 255.0f, color.a / 255.0f
	};
	glBindVertexArray(ctx->vao);
	glBindBuffer(GL_ARRAY_BUFFER, ctx->vao);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

void
draw_pixel_v(libgfx *ctx, Vec2 pos, Color color)
{
	draw_pixel(ctx, pos.x, pos.y, color);
}

void
draw_line(libgfx *ctx, int start_x, int start_y, int end_x, int end_y, Color color)
{
	if (!ctx) return;
	float vertices[12] = {
		(float)start_x, (float)start_y,
		color.r / 255.0f, color.g / 255.0f,
		color.b / 255.0f, color.a / 255.0f,
		(float)end_x, (float)end_y,
		color.r / 255.0f, color.g / 255.0f,
		color.b / 255.0f, color.a / 255.0f,
	};
	glBindVertexArray(ctx->vao);
	glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

void
draw_line_v(libgfx *ctx, Vec2 start, Vec2 end, Color color)
{
	draw_line(ctx, start.x, start.y, end.x, end.y, color);
}

// void
// draw_polygon(libgfx *ctx, Vec2 *points, int point_count, Color fill,
// 	     Color border, float border_width)
// {
// 	if (!ctx || point_count < 3) return;
//
// 	/* Stack allocate vertex data: x, y, r, g, b, a */
// 	float vertices[point_count * 6];
// 	for (int i = 0; i < point_count; i++) {
// 		vertices[i * 6 + 0] = points[i].x;
// 		vertices[i * 6 + 1] = points[i].y;
// 		vertices[i * 6 + 2] = fill.r / 255.0f;
// 		vertices[i * 6 + 3] = fill.g / 255.0f;
// 		vertices[i * 6 + 4] = fill.b / 255.0f;
// 		vertices[i * 6 + 5] = fill.a / 255.0f;
// 	}
//
// 	glBindVertexArray(ctx->vao);
// 	glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
// 		     GL_DYNAMIC_DRAW);
//
// 	glUseProgram(ctx->shader);
//
// 	/* Draw filled polygon using Triangle Fan */
// 	glDrawArrays(GL_TRIANGLE_FAN, 0, point_count);
//
// 	/* Draw border if width > 0 */
// 	if (border_width > 0.0f) {
// 		for (int i = 0; i < point_count; i++) {
// 			vertices[i * 6 + 2] = border.r / 255.0f;
// 			vertices[i * 6 + 3] = border.g / 255.0f;
// 			vertices[i * 6 + 4] = border.b / 255.0f;
// 			vertices[i * 6 + 5] = border.a / 255.0f;
// 		}
//
// 		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
// 			     GL_DYNAMIC_DRAW);
//
// 		/* NOTE
// 		 * glLineWidth > 1.0 is not supported on all platforms
// 		 * (e.g., Windows/macOS Core)
// 		 */
// 		glLineWidth(border_width);
// 		glDrawArrays(GL_LINE_LOOP, 0, point_count);
// 	}
//
// 	glBindVertexArray(0);
// }

void
draw_poly(libgfx *ctx, Vec2 center, int sides, float radius, float rotation, Color fill, Color border, float border_width)
{
	if (!ctx || sides < 3) return;
	if (sides > 1000) sides = 1000;

	Vec2 points[1000];
	float angle_step = 360.0f / sides;
	float rot_rad = rotation * (M_PI / 180.0f);

	for (int i = 0; i < sides; i++) {
		float angle = rot_rad + i * angle_step * (M_PI / 180.0f);
		points[i].x = center.x + radius * cosf(angle);
		points[i].y = center.y + radius * sinf(angle);
	}

	/* Draw Fill */
	if (fill.a > 0) {
		float vertices[1000 * 6];
		for (int i = 0; i < sides; i++) {
			vertices[i * 6 + 0] = points[i].x;
			vertices[i * 6 + 1] = points[i].y;
			vertices[i * 6 + 2] = fill.r / 255.0f;
			vertices[i * 6 + 3] = fill.g / 255.0f;
			vertices[i * 6 + 4] = fill.b / 255.0f;
			vertices[i * 6 + 5] = fill.a / 255.0f;
		}
		glBindVertexArray(ctx->vao);
		glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
		glBufferData(GL_ARRAY_BUFFER, sides * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_FAN, 0, sides);
	}

	/* Draw Border */
	if (border_width > 0.0f && border.a > 0) {
		float vertices[1000 * 36]; /* 6 vertices per segment, 6 floats per vertex */
		int v_idx = 0;
		float half_thick = border_width / 2.0f;
		float r = border.r / 255.0f;
		float g = border.g / 255.0f;
		float b = border.b / 255.0f;
		float a = border.a / 255.0f;

		for (int i = 0; i < sides; i++) {
			int next = (i + 1) % sides;
			Vec2 p1 = points[i];
			Vec2 p2 = points[next];

			Vec2 d = { p2.x - p1.x, p2.y - p1.y };
			float len = sqrtf(d.x * d.x + d.y * d.y);
			if (len == 0.0f) continue;

			Vec2 n = { -d.y / len, d.x / len };

			/* Triangle 1 */
			vertices[v_idx++] = p1.x + n.x * half_thick; vertices[v_idx++] = p1.y + n.y * half_thick;
			vertices[v_idx++] = r; vertices[v_idx++] = g; vertices[v_idx++] = b; vertices[v_idx++] = a;

			vertices[v_idx++] = p1.x - n.x * half_thick; vertices[v_idx++] = p1.y - n.y * half_thick;
			vertices[v_idx++] = r; vertices[v_idx++] = g; vertices[v_idx++] = b; vertices[v_idx++] = a;

			vertices[v_idx++] = p2.x + n.x * half_thick; vertices[v_idx++] = p2.y + n.y * half_thick;
			vertices[v_idx++] = r; vertices[v_idx++] = g; vertices[v_idx++] = b; vertices[v_idx++] = a;

			/* Triangle 2 */
			vertices[v_idx++] = p1.x - n.x * half_thick; vertices[v_idx++] = p1.y - n.y * half_thick;
			vertices[v_idx++] = r; vertices[v_idx++] = g; vertices[v_idx++] = b; vertices[v_idx++] = a;

			vertices[v_idx++] = p2.x - n.x * half_thick; vertices[v_idx++] = p2.y - n.y * half_thick;
			vertices[v_idx++] = r; vertices[v_idx++] = g; vertices[v_idx++] = b; vertices[v_idx++] = a;

			vertices[v_idx++] = p2.x + n.x * half_thick; vertices[v_idx++] = p2.y + n.y * half_thick;
			vertices[v_idx++] = r; vertices[v_idx++] = g; vertices[v_idx++] = b; vertices[v_idx++] = a;
		}

		glBindVertexArray(ctx->vao);
		glBindBuffer(GL_ARRAY_BUFFER, ctx->vbo);
		glBufferData(GL_ARRAY_BUFFER, v_idx * sizeof(float), vertices, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLES, 0, v_idx / 6);
	}

	glBindVertexArray(0);
}
