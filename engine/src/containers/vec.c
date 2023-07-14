#include <defines.h>
#include <containers/vec.h>
#include <core/mem.h>
#include <core/logger.h>

vec_t * _vec_create(u64 dimensions, u64 stride) {
    vec_t * vec = kmalloc(sizeof(vec_t), MEMORY_TAG_ARRAY);
    vec->dimensions = dimensions;
    vec->stride = stride;
    vec->array = kmalloc(dimensions * stride, MEMORY_TAG_ARRAY);

    return vec;
}

void _vec_destroy(vec_t * vec) {
    kfree(vec->array, vec->stride * vec->dimensions, MEMORY_TAG_ARRAY);
    kfree(vec, sizeof(vec_t), MEMORY_TAG_ARRAY);
}

// TODO: implement 8-byte addition and trickledown when unavailable
void _vec_add(vec_t * veca, vec_t * vecb, vec_t * out) {
    u64 stride = (out->stride <= veca->stride) ? out->stride : veca->stride;
    u8 * va = (u8 *) veca->array;
    u8 * vb = (u8 *) vecb->array;
    u8 * oa = (u8 *) out->array;
    for (u64 d = 0; d < veca->dimensions; ++d) {
        va = (u8 *) veca->array + (d * veca->stride);
        vb = (u8 *) vecb->array + (d * vecb->stride);
        oa = (u8 *) out->array + (d * out->stride);
        u8 carry = 0;
        u16 buffer = 0;
        for (u64 s = 0; s < stride; ++s) {
            buffer = va[s] + vb[s] + carry;
            oa[s] = buffer & 0xFF;
            carry = buffer >> 8;
        }
    }
}

// TODO: implement 8-byte subtraction and trickledown when unavailable
void _vec_sub(vec_t * veca, vec_t * vecb, vec_t * out) {
    u64 stride = (out->stride <= veca->stride) ? out->stride : veca->stride;
    u8 * va = (u8 *) veca->array;
    u8 * vb = (u8 *) vecb->array;
    u8 * oa = (u8 *) out->array;
    for (u64 d = 0; d < veca->dimensions; ++d) {
        va = (u8 *) veca->array + (d * veca->stride);
        vb = (u8 *) vecb->array + (d * vecb->stride);
        oa = (u8 *) out->array + (d * out->stride);
        i8 carry = 0;
        i16 buffer = 0;
        for (u64 s = 0; s < stride; ++s) {
            buffer = va[s] - vb[s] + carry;
            oa[s] = buffer & 0xFF;
            carry = buffer >> 8;
        }
    }
}

// TODO: implement 8-byte addition and trickledown when unavailable
void _vec_add_float(vec_t * veca, vec_t * vecb, vec_t * out) {
    u64 stride = (out->stride <= veca->stride) ? out->stride : veca->stride;
    if (stride != 4 && stride != 8) {
        KERROR("[_vec_add_float(veca, vecb, out)]");
        KERROR("unsupported float stride %llu", stride);
        return;
    }

    if (stride == 4) {
        f32 * va = (f32 *) veca->array;
        f32 * vb = (f32 *) vecb->array;
        f32 * oa = (f32 *) out->array;
        for (u64 d = 0; d < veca->dimensions; ++d) {
            va = (f32 *) veca->array + (d * veca->stride);
            vb = (f32 *) vecb->array + (d * vecb->stride);
            oa = (f32 *) out->array + (d * out->stride);
            for (u64 s = 0; s < stride; ++s) {
                oa[s] = va[s] + vb[s];
            }
        }
        return;
    }
}

// TODO: implement 8-byte addition and trickledown when unavailable
void _vec_sub_float(vec_t * veca, vec_t * vecb, vec_t * out) {
    u64 stride = (out->stride <= veca->stride) ? out->stride : veca->stride;
    if (stride != 4 && stride != 8) {
        KERROR("[_vec_sub_float(veca, vecb, out)]");
        KERROR("unsupported float stride %llu", stride);
        return;
    }

    if (stride == 4) {
        f32 * va = (f32 *) veca->array;
        f32 * vb = (f32 *) vecb->array;
        f32 * oa = (f32 *) out->array;
        for (u64 d = 0; d < veca->dimensions; ++d) {
            va = (f32 *) veca->array + (d * veca->stride);
            vb = (f32 *) vecb->array + (d * vecb->stride);
            oa = (f32 *) out->array + (d * out->stride);
            for (u64 s = 0; s < stride; ++s) {
                oa[s] = va[s] - vb[s];
            }
        }
        return;
    }
}