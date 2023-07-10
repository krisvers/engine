#include <renderer/frontend.h>
#include <renderer/backend.h>
#include <core/logger.h>
#include <core/mem.h>
#include <defines.h>

static renderer_backend_t * backend = NULL;

b8 renderer_init(const char * application_name, platform_state_t * pstate) {
    backend = kmalloc(sizeof(renderer_backend_t), MEMORY_TAG_RENDERER);

    renderer_backend_create(RENDERER_BACKEND_VULKAN, pstate, backend);
    backend->frame_number = 0;

    if (!backend->init(backend, application_name, pstate)) {
        KFATAL("[renderer_init(application_name, pstate)]");
        KFATAL("renderer backend failed to initialized");
        return FALSE;
    }

    return TRUE;
}

void renderer_deinit(void) {
    backend->deinit(backend);
    kfree(backend, sizeof(renderer_backend_t), MEMORY_TAG_RENDERER);
}

void renderer_on_resize(u32 w, u32 h) {

}

b8 renderer_frame_begin(f64 delta_time) {
    return backend->frame_begin(backend, delta_time);
}

b8 renderer_frame_end(f64 delta_time) {
    ++backend->frame_number;
    return backend->frame_end(backend, delta_time);
}

b8 renderer_draw_frame(render_packet_t * packet) {
    if (!renderer_frame_begin(packet->delta_time)) {
        return TRUE;
    }

    b8 result = renderer_frame_end(packet->delta_time);
    if (!result) {
        KFATAL("[renderer_draw_frame(packet)]");
        KFATAL("failed to end renderer frame");
        return FALSE;
    }

    return TRUE;
}