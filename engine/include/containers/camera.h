#ifndef KENGINE_CONTAINERS_CAMERA_H
#define KENGINE_CONTAINERS_CAMERA_H

#include <defines.h>
#include <containers/transform.h>
#include <math/linmath.h>

typedef struct camera {
    transform_t transform;
    f32 clipping_near;
    f32 clipping_far;
    f32 fov;
    vec4 edges;
    b8 ortho;
    float ratio;
} camera_t;

camera_t KAPI * camera_create(f32 fov, f32 near, f32 far, f32 w, f32 h);
void KAPI camera_destroy(camera_t * cam);
void camera_view_matrix(camera_t * cam, mat4x4 R);
void camera_perspective_matrix(camera_t * cam, mat4x4 R);
void camera_update_resolution(camera_t * cam, u32 width, u32 height);
camera_t KAPI * ortho_camera_create(float w, float h, float near, float far);

#endif