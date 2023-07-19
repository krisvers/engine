#include <defines.h>

#ifdef KPLATFORM_DIRECTX

#error "DirectX not supported"

#include <renderer/directx/backend.h>
#include <renderer/renderer_types.h>

b8 directx_renderer_backend_init(renderer_backend_t * backend, const char * application_name, platform_state_t * pstate) {
	return FALSE;
}

void directx_renderer_backend_deinit(renderer_backend_t * backend) {

}

void directx_renderer_backend_on_resize(renderer_backend_t * backend, u32 w, u32 h) {

}

b8 directx_renderer_backend_frame_begin(renderer_backend_t * backend, render_packet_t * packet) {
	return FALSE;
}

b8 directx_renderer_backend_frame_end(renderer_backend_t * backend, render_packet_t * packet) {
	return FALSE;
}

#endif