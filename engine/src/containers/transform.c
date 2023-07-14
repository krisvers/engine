#include <containers/transform.h>
#include <math/linmath.h>
#include <math.h>

void transform_forward_yaw(transform_t * transform, vec3 R) {
	R[0] = sinf(transform->rotation[1] * (3.141592f / 180.0f));
	R[1] = 0;
	R[2] = cosf(transform->rotation[1] * (3.141592f / 180.0f));
}

void transform_right_yaw(transform_t * transform, vec3 forward, vec3 R) {
    vec3_mul_cross(R, transform->up, forward);
}