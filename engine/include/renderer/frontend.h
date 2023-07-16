#ifndef KENGINE_RENDERER_FRONTEND_H
#define KENGINE_RENDERER_FRONTEND_H

#include <defines.h>
#include <platform/platform.h>
#include <renderer/renderer_types.h>

b8 renderer_init(const char * application_name, platform_state_t * pstate);
void renderer_deinit(void);
void renderer_on_resize(u32 w, u32 h);
b8 renderer_draw_frame(render_packet_t * packet);
void renderer_set_camera(camera_t * cam);

#endif