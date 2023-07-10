#ifndef KENGINE_RENDERER_BACKEND_H
#define KENGINE_RENDERER_BACKEND_H

#include <defines.h>
#include <renderer/renderer_types.h>
#include <platform/platform.h>

b8 renderer_backend_create(renderer_backends_enum type, platform_state_t * pstate, renderer_backend_t * out_backend);
void renderer_backend_destroy(renderer_backend_t * backend);

#endif