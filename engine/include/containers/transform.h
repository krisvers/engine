#ifndef KENGINE_CONTAINERS_TRANSFORM_H
#define KENGINE_CONTAINERS_TRANSFORM_H

#include <defines.h>
#include <math/linmath.h>

typedef struct transform {
    vec3 position;
    vec3 rotation;
    vec3 up;
} transform_t;

void transform_forward(transform_t * transform, vec3 R);

#endif