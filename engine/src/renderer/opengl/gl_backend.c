#include <defines.h>

#ifdef KPLATFORM_OPENGL

#include <renderer/opengl/backend.h>
#include <renderer/opengl/opengl_types.h>
#include <renderer/renderer_types.h>
#include <platform/platform.h>
#include <glad/glad.h>
#include <core/logger.h>
#include <core/mem.h>
#include <core/file.h>
#include <math/linmath.h>
#include <containers/camera.h>
#include <containers/mesh.h>
#include <containers/vertex.h>
#include <containers/texture.h>
#include <file/tga.h>
#include <stdio.h>

// TODO: re-fucking-factor this!
#ifdef KPLATFORM_WINDOWS
#include <malloc.h>
#define alloca _malloca
#else
#include <stdlib.h>
#endif

static opengl_context_t context;

vec3 position = { 0.0f, 0.0f, -5.0f };
vec3 rotation = { 0, 0, 0 };
vec3 scale = { 1, 1, 1 };

#define SHADER_VERTEX_FILE "./assets/vertex.glsl"
#define SHADER_FRAGMENT_FILE "./assets/fragment.glsl"

static b8 opengl_init_shaders(void) {
	FILE * vertfp = fopen(SHADER_VERTEX_FILE, "rb");
	if (vertfp == NULL) {
		KERROR("[open_load_shaders()]");
		KERROR("failed to open vertex shader %s", SHADER_VERTEX_FILE);
		return FALSE;
	}

	FILE * fragfp = fopen(SHADER_FRAGMENT_FILE, "rb");
	if (fragfp == NULL) {
		KERROR("[open_load_shaders()]");
		KERROR("failed to open fragment shader %s", SHADER_FRAGMENT_FILE);
		return FALSE;
	}

	u64 vertlen;
	fseek(vertfp, 0L, SEEK_END);
	vertlen = ftell(vertfp);
	fseek(vertfp, 0L, SEEK_SET);

	u64 fraglen;
	fseek(fragfp, 0L, SEEK_END);
	fraglen = ftell(fragfp);
	fseek(fragfp, 0L, SEEK_SET);

	char * shader_source_vert = alloca(vertlen + 1);
	char * shader_source_frag = alloca(fraglen + 1);
	if (fread(shader_source_vert, vertlen, 1, vertfp) != 1) {
		KERROR("[opengl_load_shaders()]");
		KERROR("failure while reading vertex shader from %s", SHADER_VERTEX_FILE);
		return FALSE;
	}
	if (fread(shader_source_frag, fraglen, 1, fragfp) != 1) {
		KERROR("[opengl_load_shaders()]");
		KERROR("failure while reading fragment shader from %s", SHADER_FRAGMENT_FILE);
		return FALSE;
	}
	shader_source_vert[vertlen] = '\0';
	shader_source_frag[fraglen] = '\0';

	fclose(vertfp);
	fclose(fragfp);

	context.shader_vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(context.shader_vertex, 1, (const char * const *) &shader_source_vert, (const GLint *) NULL);
	glCompileShader(context.shader_vertex);

	GLint result = GL_TRUE;
	glGetShaderiv(context.shader_vertex, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		char log[256];
		glGetShaderInfoLog(context.shader_vertex, 256, (GLsizei *) NULL, log);
		KERROR("[opengl_compile_shaders()]");
		KERROR("failed to compile opengl vertex shader:");
		KERROR("%s", log);
		return FALSE;
	}

	context.shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(context.shader_fragment, 1, (const char * const *) &shader_source_frag, (const GLint *) NULL);
	glCompileShader(context.shader_fragment);

	glGetShaderiv(context.shader_fragment, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		char log[256];
		glGetShaderInfoLog(context.shader_fragment, 256, (GLsizei *) NULL, log);
		KERROR("[opengl_compile_shaders()]");
		KERROR("failed to compile opengl fragment shader:");
		KERROR("%s", log);
		return FALSE;
	}

	context.shader = glCreateProgram();
	glAttachShader(context.shader, context.shader_vertex);
	glAttachShader(context.shader, context.shader_fragment);
	glLinkProgram(context.shader);

	glGetProgramiv(context.shader, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		char log[256];
		glGetProgramInfoLog(context.shader, 256, (GLsizei *) NULL, log);
		KERROR("[opengl_compile_shaders()]");
		KERROR("failed to link opengl shader program:");
		KERROR("%s", log);
		return FALSE;
	}

	glDetachShader(context.shader, context.shader_vertex);
	glDetachShader(context.shader, context.shader_fragment);
	glDeleteShader(context.shader_vertex);
	glDeleteShader(context.shader_fragment);

	return TRUE;
}

#define MAX_VERTICES 1024
#define MAX_INDICES 8192

b8 opengl_renderer_backend_init(renderer_backend_t * backend, const char * application_name, platform_state_t * pstate) {
	if (!opengl_init_shaders()) {
		KERROR("[opengl_renderer_backend_init(backend, application_name, pstate)]");
		KERROR("failed to init shaders");
		return FALSE;
	}

	glGenVertexArrays(1, &context.vao);
	glGenBuffers(1, &context.vbo);
	glGenBuffers(1, &context.ebo);

	glBindVertexArray(context.vao);
	glBindBuffer(GL_ARRAY_BUFFER, context.vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(vertex_t), NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_INDICES * sizeof(indice_t), NULL, GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer(0, VERTEX_POSITION_SIZE, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void *) VERTEX_POSITION_OFFSET);
	glVertexAttribPointer(1, VERTEX_COLOR_SIZE, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void *) VERTEX_COLOR_OFFSET);
	glVertexAttribPointer(2, VERTEX_UV_SIZE, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void *) VERTEX_UV_OFFSET);
	glVertexAttribPointer(3, VERTEX_TEX_SIZE, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (const void *) VERTEX_TEX_OFFSET);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);

	context.mvp = glGetUniformLocation(context.shader, "in_mvp");

	glGenTextures(1, &context.texture);
	glBindTexture(GL_TEXTURE_2D, context.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	file_t file;
	file_open(&file, (char *) "./assets/test.tga", FILE_READ);
	file_read(&file);
	tga_t tga;
	tga.texture.buffer = (u8 *) NULL;
	tga_empty(&tga);
	tga_load(&tga, &file);

	texture_atlas_t * atlas = texture_atlas_create();
	texture_atlas_stitch(atlas, &tga.texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_atlas_at(atlas, 0)->width, texture_atlas_at(atlas, 0)->height, 0, GL_BGRA, GL_UNSIGNED_BYTE, atlas->buffer);

	file_close(&file);

	file_open(&file, (char *) "./assets/written.tga", FILE_WRITE);
	tga_save(&tga, &file);

	file_write(&file);
	file_close(&file);

	tga_empty(&tga);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	return TRUE;
}

void opengl_renderer_backend_deinit(renderer_backend_t * backend) {
	glDeleteProgram(context.shader);
}

void opengl_renderer_backend_on_resize(renderer_backend_t * backend, u32 w, u32 h) {
	if (backend->camera != NULL) {
		camera_update_resolution(backend->camera, w, h);
	}
	glViewport(0, 0, w, h);
}

b8 opengl_renderer_backend_frame_begin(renderer_backend_t * backend, render_packet_t * packet) {
	if (backend->camera == NULL) {
		KERROR("[opengl_renderer_backend_frame_begin(backend, delta_time)]");
		KERROR("no camera assigned");
		return TRUE;
	}

	glBindBuffer(GL_ARRAY_BUFFER, context.vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, packet->mesh->vertices->length * sizeof(vertex_t), packet->mesh->vertices->array);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.ebo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, packet->mesh->indices->length * sizeof(indice_t), packet->mesh->indices->array);

	mat4x4 m, v, p, mvp;
	mat4x4_identity(m);
	mat4x4_identity(v);
	mat4x4_identity(p);
	mat4x4_identity(mvp);

	mat4x4_scale_aniso(m, m, scale[0], scale[1], scale[2]);
	mat4x4_rotate_X(m, m, rotation[0] * (3.141592f / 180.0f));
	mat4x4_rotate_Y(m, m, rotation[1] * (3.141592f / 180.0f));
	mat4x4_rotate_Z(m, m, rotation[2] * (3.141592f / 180.0f));
	mat4x4_translate(m, position[0], position[1], position[2]);

	camera_view_matrix(backend->camera, v);
	camera_perspective_matrix(backend->camera, p);

	mat4x4_mul(mvp, mvp, p);
	mat4x4_mul(mvp, mvp, v);
	mat4x4_mul(mvp, mvp, m);

	glUniformMatrix4fv(context.mvp, 1, GL_FALSE, &mvp[0][0]);
	glBindTexture(GL_TEXTURE_2D, context.texture);
	return TRUE;
}

b8 opengl_renderer_backend_frame_end(renderer_backend_t * backend, render_packet_t * packet) {
	if (backend->camera == NULL) {
		KERROR("[opengl_renderer_backend_frame_end(backend, delta_time)]");
		KERROR("no camera assigned");
		return FALSE;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.12, 0.07, 0.12, 1.0);
	glUseProgram(context.shader);
	glDrawElements(GL_TRIANGLES, packet->mesh->indices->length * sizeof(indice_t), GL_UNSIGNED_INT, 0);
	return TRUE;
}

#endif