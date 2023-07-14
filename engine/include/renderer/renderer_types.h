#ifndef KENGINE_RENDERER_RENDERER_TYPES_H
#define KENGINE_RENDERER_RENDERER_TYPES_H

#include <defines.h>
#include <platform/platform.h>
#include <containers/camera.h>

typedef enum rendererBackendsEnum {
    RENDERER_BACKEND_VULKAN,
    RENDERER_BACKEND_OPENGL,
    RENDERER_BACKEND_DIRECTX,
    RENDERER_BACKEND_SOFTWARE,
} renderer_backends_enum;

typedef struct rendererBackend {
    platform_state_t * platform_state;
    u64 frame_number;
    b8 (* init)(struct rendererBackend * backend, const char * application_name, platform_state_t * pstate);
    void (* deinit)(struct rendererBackend * backend);
    void (* resize)(struct rendererBackend * backend, u32 w, u32 h);
    b8 (* frame_begin)(struct rendererBackend * backend, f64 delta_time);
    b8 (* frame_end)(struct rendererBackend * backend, f64 delta_time);
    camera_t * camera;
} renderer_backend_t;

typedef b8 (* renderer_init_func)(renderer_backend_t * backend, const char * application_name, platform_state_t * pstate);
typedef void (* renderer_deinit_func)(renderer_backend_t * backend);
typedef void (* renderer_resize_func)(renderer_backend_t * backend, u32 w, u32 h);
typedef b8 (* renderer_frame_func)(renderer_backend_t * backend, f64 delta_time);

typedef struct renderPacket {
    f64 delta_time;
} render_packet_t;

#endif