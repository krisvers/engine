#ifndef KENGINE_CONTAINERS_TRANSFORM_H
#define KENGINE_CONTAINERS_TRANSFORM_H

#include <defines.h>
#include <math/linmath.h>

typedef struct transform {
    vec3 position;
    vec3 rotation;
    vec3 up;
} transform_t;

void KAPI transform_forward_yaw(transform_t * transform, vec3 R);
void KAPI transform_right_yaw(transform_t * transform, vec3 forward, vec3 R);

#endif