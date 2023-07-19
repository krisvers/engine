#include <renderer/frontend.h>
#include <renderer/backend.h>
#include <containers/camera.h>
#include <core/logger.h>
#include <core/mem.h>
#include <defines.h>

static renderer_backend_t * backend = (renderer_backend_t *) NULL;

b8 renderer_init(const char * application_name, platform_state_t * pstate) {
    backend = (renderer_backend_t *) kmalloc(sizeof(renderer_backend_t), MEMORY_TAG_RENDERER);

    if (!renderer_backend_create(RENDERER_BACKEND_OPENGL, pstate, backend)) {
        KFATAL("[renderer_init(application_name, pstate)]");
        KFATAL("failed to create renderer backend");
        return FALSE;
    }
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
    backend->resize(backend, w, h);
}

b8 renderer_frame_begin(render_packet_t * packet) {
    return backend->frame_begin(backend, packet);
}

b8 renderer_frame_end(render_packet_t * packet) {
    ++backend->frame_number;
    return backend->frame_end(backend, packet);
}

b8 renderer_draw_frame(render_packet_t * packet) {
    if (!renderer_frame_begin(packet)) {
        return TRUE;
    }

    b8 result = renderer_frame_end(packet);
    if (!result) {
        KERROR("[renderer_draw_frame(packet)]");
        KERROR("failed to end renderer frame");
        return FALSE;
    }

    return TRUE;
}

void renderer_set_camera(camera_t * cam) {
    backend->camera = cam;
}