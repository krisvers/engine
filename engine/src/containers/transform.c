#include <containers/transform.h>

void transform_forward_yaw(transform_t * transform, vec3 R) {
	R[0] = sinf(transform.rotation[1] * (3.141592f / 180.0f));
	R[1] = 0;
	R[2] = cosf(transform.rotation[1] * (3.141592f / 180.0f));
}