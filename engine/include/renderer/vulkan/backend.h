#ifndef KENGINE_RENDERER_VULKAN_BACKEND_H
#define KENGINE_RENDERER_VULKAN_BACKEND_H

#include <renderer/renderer_types.h>
#include <platform/platform.h>

b8 vulkan_renderer_backend_init(renderer_backend_t * backend, const char * application_name, platform_state_t * pstate);
void vulkan_renderer_backend_deinit(renderer_backend_t * backend);
void vulkan_renderer_backend_on_resize(renderer_backend_t * backend, u32 w, u32 h);
b8 vulkan_renderer_backend_frame_begin(renderer_backend_t * backend, f64 delta_time);
b8 vulkan_renderer_backend_frame_end(renderer_backend_t * backend, f64 delta_time);

#endif