#ifndef KENGINE_RENDERER_OPENGL_OPENGL_TYPES_H
#define KENGINE_RENDERER_OPENGL_OPENGL_TYPES_H

#include <defines.h>
#include <glad/glad.h>
#include <containers/camera.h>
#include <math/linmath.h>

typedef struct openglContext {
	GLuint vbo;
	GLuint vao;
	GLuint ebo;
	GLuint shader;
	GLuint shader_vertex;
	GLuint shader_fragment;
	GLint matrix;
	mat4x4 mvp;
} opengl_context_t;

#endif