#include <renderer/backend.h>
#include <defines.h>
#include <renderer/vulkan/backend.h>

b8 renderer_backend_create(renderer_backends_enum type, platform_state_t * pstate, renderer_backend_t * out_backend) {
    out_backend->platform_state = pstate;

    if (type == RENDERER_BACKEND_VULKAN) {
        out_backend->init = vulkan_renderer_backend_init;
        out_backend->deinit = vulkan_renderer_backend_deinit;
        out_backend->frame_begin = vulkan_renderer_backend_frame_begin;
        out_backend->frame_end = vulkan_renderer_backend_frame_end;
        out_backend->resize = vulkan_renderer_backend_on_resize;
        return TRUE;
    }

    return FALSE;
}

void renderer_backend_destroy(renderer_backend_t * backend) {
    backend->init = NULL;
    backend->deinit = NULL;
    backend->frame_begin = NULL;
    backend->frame_end = NULL;
    backend->resize = NULL;
}