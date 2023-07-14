#ifndef KENGINE_CONTAINERS_VEC_H
#define KENGINE_CONTAINERS_VEC_H

#include <defines.h>

typedef struct vec {
    u64 stride;
    u64 dimensions;
    void * array;
} vec_t;

#define vec_at(vec, index, type) *((type *) vec->array + (index * vec->stride))

vec_t KAPI * _vec_create(u64 dimensions, u64 stride);
void KAPI _vec_destroy(vec_t * vec);

void KAPI _vec_add(vec_t * veca, vec_t * vecb, vec_t * out);
void KAPI _vec_sub(vec_t * veca, vec_t * vecb, vec_t * out);
void KAPI _vec_mul(vec_t * veca, vec_t * vecb, vec_t * out);
void KAPI _vec_div(vec_t * veca, vec_t * vecb, vec_t * out);

void _vec_add_float(vec_t * veca, vec_t * vecb, vec_t * out);
void _vec_sub_float(vec_t * veca, vec_t * vecb, vec_t * out);

void KAPI _vec_normalized(vec_t * vec, vec_t * out);
f64 KAPI _vec_magnitude(vec_t * vec);

#endif