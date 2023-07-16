#include <containers/camera.h>
#include <containers/transform.h>
#include <defines.h>
#include <core/mem.h>
#include <math.h>

camera_t * camera_create(f32 fov, f32 near, f32 far, f32 w, f32 h) {
	camera_t * cam = kmalloc(sizeof(camera_t), MEMORY_TAG_CAMERA);
	kmemzero(&cam->transform, sizeof(transform_t));
	cam->transform.up[0] = 0.0f; cam->transform.up[1] = 1.0f; cam->transform.up[2] = 0.0f;
	cam->clipping_far = far;
	cam->clipping_near = near;
	cam->fov = fov;
	cam->ortho = FALSE;
	cam->ratio = w / (float) h;

	return cam;
}

void camera_destroy(camera_t * cam) {
	kfree(cam, sizeof(camera_t), MEMORY_TAG_CAMERA);
}

void camera_view_matrix(camera_t * cam, mat4x4 R) {
	mat4x4_translate(R, -cam->transform.position[0], -cam->transform.position[1], cam->transform.position[2]);
}

void camera_update_resolution(camera_t * cam, u32 width, u32 height) {
	cam->ratio = width / (f32) height;
	if (!cam->ortho) {
		return;
	}

	cam->edges[0] = -cam->ratio; cam->edges[1] = cam->ratio; cam->edges[2] = -1; cam->edges[3] = 1;
}

void camera_perspective_matrix(camera_t * cam, mat4x4 R) {
	if (cam->ortho) {
		mat4x4_ortho(R, cam->edges[0], cam->edges[1], cam->edges[2], cam->edges[3], cam->clipping_near, cam->clipping_far);
		mat4x4_rotate_X(R, R, -cam->transform.rotation[0] * (3.141592f / 180.0f));
		mat4x4_rotate_Y(R, R, cam->transform.rotation[1] * (3.141592f / 180.0f));
		mat4x4_rotate_Z(R, R, cam->transform.rotation[2] * (3.141592f / 180.0f));
		return;
	}
	mat4x4_perspective(R, cam->fov * (3.141592 / 180.0f), cam->ratio, cam->clipping_near, cam->clipping_far);
	mat4x4_rotate_X(R, R, -cam->transform.rotation[0] * (3.141592f / 180.0f));
	mat4x4_rotate_Y(R, R, cam->transform.rotation[1] * (3.141592f / 180.0f));
	mat4x4_rotate_Z(R, R, cam->transform.rotation[2] * (3.141592f / 180.0f));
}

camera_t * ortho_camera_create(float w, float h, float near, float far) {
	camera_t * cam = kmalloc(sizeof(camera_t), MEMORY_TAG_CAMERA);
	kmemzero(&cam->transform, sizeof(transform_t));
	cam->ortho = TRUE;
	cam->transform.up[0] = 0; cam->transform.up[1] = 1; cam->transform.up[2] = 0;
	cam->ratio = w / (float) h;
	cam->edges[0] = -cam->ratio; cam->edges[1] = cam->ratio; cam->edges[2] = -1; cam->edges[3] = 1;
	cam->clipping_far = far;
	cam->clipping_near = near;

	return cam;
}