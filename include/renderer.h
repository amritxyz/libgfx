/* See LICENSE file for copyright and license details. */
#pragma once

#include <GL/gl.h>

#include "libgfx.h"

typedef struct {
	GLuint program;
	GLint  color_loc;
	GLint  pos_matrix_loc;
} shader_program_t;

typedef struct {
	GLuint VAO, VBO, EBO;
	int index_count;
} mesh_t;

shader_program_t *shader_create(const char *vert_src, const char *frag_src);
void shader_use(shader_program_t *shader);
void shader_destroy(shader_program_t *shader);

mesh_t *mesh_create(float *vertices, int vertex_count, unsigned int *indices,
		    int index_count);
void mesh_render(mesh_t *mesh);
void mesh_destroy(mesh_t *mesh);
