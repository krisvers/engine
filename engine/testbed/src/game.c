#include <defines.h>
#include <game.h>
#include <core/logger.h>
#include <core/input.h>
#include <core/event.h>
#include <containers/dynarray.h>

static b8 on_key_press(u16 code, void * sender, void * listener, event_context_t ctx) {
	KLOG("key press %u, %s", ctx.data.u16[0], input_get_key_cstr(ctx.data.u16[0]));

	return FALSE;
}

static b8 on_mouse_move(u16 code, void * sender, void * listener, event_context_t ctx) {
	KLOG("mouse move %i, %i", ctx.data.i32[0], ctx.data.i32[1]);

	return FALSE;
}

static b8 on_mouse_press(u16 code, void * sender, void * listener, event_context_t ctx) {
	KLOG("mouse press %u, %s", ctx.data.u8[0], input_get_mouse_button_cstr(ctx.data.u8[0]));

	return FALSE;
}

static b8 on_mouse_scroll(u16 code, void * sender, void * listener, event_context_t ctx) {
	KLOG("mouse scroll %i, %i", ctx.data.i8[0], ctx.data.i8[1]);

	return FALSE;
}

b8 game_initialize(game_t * instance) {
	(void) instance;

	event_register(EVENT_CODE_KEY_PRESSED, (void *) game_initialize, on_key_press);
	event_register(EVENT_CODE_MOUSE_MOVED, (void *) game_initialize, on_mouse_move);
	event_register(EVENT_CODE_MOUSE_SCROLL, (void *) game_initialize, on_mouse_scroll);
	event_register(EVENT_CODE_MOUSE_BUTTON_PRESSED, (void *) game_initialize, on_mouse_press);

	return TRUE;
}

b8 game_update(game_t * instance, f64 delta_time) {
	(void) instance;
	(void) delta_time;

	event_context_t ctx;
	ctx.data.i32[0] = 4; 
	//event_fire(69, game_update, ctx);

	return TRUE;
}

b8 game_render(game_t * instance, f64 delta_time) {
	(void) instance;
	(void) delta_time;
	return TRUE;
}
