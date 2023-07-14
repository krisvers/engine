#include <defines.h>

#ifdef KPLATFORM_OPENGL

#include <renderer/opengl/backend.h>
#include <renderer/opengl/opengl_types.h>
#include <renderer/renderer_types.h>
#include <platform/platform.h>
#include <glad/glad.h>
#include <core/logger.h>
#include <core/mem.h>
#include <math/linmath.h>
#include <containers/vec.h>
#include <containers/camera.h>
#include <stdio.h>

static opengl_context_t context;

typedef struct vertex {
	vec3 position;
	vec3 color;
} vertex_t;

float vertices[] = {
	1, 1, 0, 1, 1, 0,
	1, 0, 0, 0, 1, 1,
	0, 1, 0, 1, 0, 1,
};

u32 indices[] = {
	0, 1, 2,
	3, 4, 5
};

vec3 position = { 0.0f, 0.0f, -5.0f };
vec3 rotation = { 0, 0, 0 };
vec3 scale = { 1, 1, 1 };

char * shader_source_vert = NULL;
char * shader_source_frag = NULL;

#define SHADER_VERTEX_FILE "./rsrc/vertex.glsl"
#define SHADER_FRAGMENT_FILE "./rsrc/fragment.glsl"

static b8 opengl_load_shaders(void) {
	FILE * vertfp = fopen(SHADER_VERTEX_FILE, "r");
	if (vertfp == NULL) {
		KERROR("[open_load_shaders()]");
		KERROR("failed to open vertex shader %s", SHADER_VERTEX_FILE);
		return FALSE;
	}

	FILE * fragfp = fopen(SHADER_FRAGMENT_FILE, "r");
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

	shader_source_vert = kmalloc(vertlen + 1, MEMORY_TAG_RENDERER);
	shader_source_frag = kmalloc(fraglen + 1, MEMORY_TAG_RENDERER);
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

	return TRUE;
}

static b8 opengl_compile_shaders(void) {
	context.shader_vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(context.shader_vertex, 1, (const char * const *) &shader_source_vert, NULL);
	glCompileShader(context.shader_vertex);

	GLint result = GL_TRUE;
	glGetShaderiv(context.shader_vertex, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		char log[256];
		glGetShaderInfoLog(context.shader_vertex, 256, NULL, log);
		KERROR("[opengl_compile_shaders()]");
		KERROR("failed to compile opengl vertex shader:");
		KERROR("%s", log);
		return FALSE;
	}

	context.shader_fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(context.shader_fragment, 1, (const char * const *) &shader_source_frag, NULL);
	glCompileShader(context.shader_fragment);

	glGetShaderiv(context.shader_fragment, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		char log[256];
		glGetShaderInfoLog(context.shader_fragment, 256, NULL, log);
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
		glGetProgramInfoLog(context.shader, 256, NULL, log);
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

b8 opengl_renderer_backend_init(renderer_backend_t * backend, const char * application_name, platform_state_t * pstate) {
	if (!opengl_load_shaders()) {
		KERROR("[opengl_renderer_backend_init(backend, application_name, pstate)]");
		KERROR("failed to load opengl shaders");
		return FALSE;
	}

	if (!opengl_compile_shaders()) {
		KERROR("[opengl_renderer_backend_init(backend, application_name, pstate)]");
		KERROR("failed to compile opengl shaders");
		return FALSE;
	}

	glGenVertexArrays(1, &context.vao);
	glGenBuffers(1, &context.vbo);
	glGenBuffers(1, &context.ebo);

	glBindVertexArray(context.vao);
	glBindBuffer(GL_ARRAY_BUFFER, context.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (const void *) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	context.matrix = glGetUniformLocation(context.shader, "in_mvp");

	return TRUE;
}

void opengl_renderer_backend_deinit(renderer_backend_t * backend) {
	glDeleteProgram(context.shader);
}

void opengl_renderer_backend_on_resize(renderer_backend_t * backend, u32 w, u32 h) {
	backend->camera->ratio = w / (f32) h;
	glViewport(0, 0, w, h);
}

b8 opengl_renderer_backend_frame_begin(renderer_backend_t * backend, f64 delta_time) {
	if (backend->camera == NULL) {
		KERROR("[opengl_renderer_backend_frame_begin(backend, delta_time)]");
		KERROR("no camera assigned");
		return TRUE;
	}

	mat4x4 m, v, p;
	mat4x4_identity(m);
	mat4x4_identity(v);
	mat4x4_identity(p);
	mat4x4_identity(context.mvp);

	mat4x4_translate(m, position[0], position[1], position[2]);
	mat4x4_scale_aniso(m, m, scale[0], scale[1], scale[2]);
	mat4x4_rotate_X(m, m, rotation[0] * (3.141592f / 180.0f));
	mat4x4_rotate_Y(m, m, rotation[1] * (3.141592f / 180.0f));
	mat4x4_rotate_Z(m, m, rotation[2] * (3.141592f / 180.0f));

	camera_view_matrix(backend->camera, v);

	camera_perspective_matrix(backend->camera, p);

	mat4x4_mul(context.mvp, p, v);
	mat4x4_mul(context.mvp, context.mvp, m);
	
	glUniformMatrix4fv(context.matrix, 1, GL_FALSE, &context.mvp[0][0]);
	return TRUE;
}

b8 opengl_renderer_backend_frame_end(renderer_backend_t * backend, f64 delta_time) {
	if (backend->camera == NULL) {
		KERROR("[opengl_renderer_backend_frame_end(backend, delta_time)]");
		KERROR("no camera assigned");
		return FALSE;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.11, 0.13, 0.12, 1.0);
	glUseProgram(context.shader);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, context.ebo);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	return TRUE;
}

#endif