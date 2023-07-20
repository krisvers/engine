#ifndef KENGINE_RENDERER_RENDERER_TYPES_H
#define KENGINE_RENDERER_RENDERER_TYPES_H

#include <defines.h>
#include <platform/platform.h>
#include <containers/camera.h>
#include <containers/texture.h>
#include <containers/mesh.h>

typedef enum rendererBackendsEnum {
    RENDERER_BACKEND_VULKAN,
    RENDERER_BACKEND_OPENGL,
    RENDERER_BACKEND_DIRECTX,
    RENDERER_BACKEND_SOFTWARE,
} renderer_backends_enum;

typedef struct renderPacket {
    f64 delta_time;
    mesh_t * mesh;
    texture_atlas_t * atlas;
} render_packet_t;

typedef struct rendererBackend {
    platform_state_t * platform_state;
    u64 frame_number;
    b8 (* init)(struct rendererBackend * backend, const char * application_name, platform_state_t * pstate);
    void (* deinit)(struct rendererBackend * backend);
    void (* resize)(struct rendererBackend * backend, u32 w, u32 h);
    b8 (* frame_begin)(struct rendererBackend * backend, render_packet_t * packet);
    b8 (* frame_end)(struct rendererBackend * backend, render_packet_t * packet);
    camera_t * camera;
} renderer_backend_t;

typedef b8 (* renderer_init_func)(renderer_backend_t * backend, const char * application_name, platform_state_t * pstate);
typedef void (* renderer_deinit_func)(renderer_backend_t * backend);
typedef void (* renderer_resize_func)(renderer_backend_t * backend, u32 w, u32 h);
typedef b8 (* renderer_frame_func)(renderer_backend_t * backend, render_packet_t * packet);

#endif