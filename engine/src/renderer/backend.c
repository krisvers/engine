#include <renderer/backend.h>
#include <defines.h>

#ifdef KPLATFORM_VULKAN
#include <renderer/vulkan/backend.h>
#endif

#ifdef KPLATFORM_OPENGL
#include <renderer/opengl/backend.h>
#endif

b8 renderer_backend_create(renderer_backends_enum type, platform_state_t * pstate, renderer_backend_t * out_backend) {
    out_backend->platform_state = pstate;

#ifdef KPLATFORM_VULKAN
    if (type == RENDERER_BACKEND_VULKAN) {
        out_backend->init = vulkan_renderer_backend_init;
        out_backend->deinit = vulkan_renderer_backend_deinit;
        out_backend->frame_begin = vulkan_renderer_backend_frame_begin;
        out_backend->frame_end = vulkan_renderer_backend_frame_end;
        out_backend->resize = vulkan_renderer_backend_on_resize;
        return TRUE;
    }
#endif
#ifdef KPLATFORM_OPENGL
    if (type == RENDERER_BACKEND_OPENGL) {
        out_backend->init = opengl_renderer_backend_init;
        out_backend->deinit = opengl_renderer_backend_deinit;
        out_backend->frame_begin = opengl_renderer_backend_frame_begin;
        out_backend->frame_end = opengl_renderer_backend_frame_end;
        out_backend->resize = opengl_renderer_backend_on_resize;
        return TRUE;
    }
#endif

    return FALSE;
}

void renderer_backend_destroy(renderer_backend_t * backend) {
    backend->init = (renderer_init_func) NULL;
    backend->deinit = (renderer_deinit_func) NULL;
    backend->frame_begin = (renderer_frame_func) NULL;
    backend->frame_end = (renderer_frame_func) NULL;
    backend->resize = (renderer_resize_func) NULL;
}