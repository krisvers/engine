#include <defines.h>
#include <game.h>
#include <core/logger.h>
#include <core/input.h>
#include <core/event.h>
#include <containers/dynarray.h>
#include <math/linmath.h>
#include <math.h>

game_t * this = NULL;

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
	this = instance;

	event_register(EVENT_CODE_KEY_PRESSED, (void *) game_initialize, on_key_press);
	event_register(EVENT_CODE_MOUSE_MOVED, (void *) game_initialize, on_mouse_move);
	event_register(EVENT_CODE_MOUSE_SCROLL, (void *) game_initialize, on_mouse_scroll);
	event_register(EVENT_CODE_MOUSE_BUTTON_PRESSED, (void *) game_initialize, on_mouse_press);

	return TRUE;
}

b8 game_update(game_t * instance, f64 delta_time) {
	(void) instance;
	(void) delta_time;

	float base_speed = 0.75f;
	float slow_speed = 0.1f;
	float fast_speed = 4.0f;
	float speed = base_speed;
	float sensitivity = 1.0f;

	vec3 forward;
	forward[0] = sinf(this->camera->transform.rotation[1]);
	forward[1] = 0;
	forward[2] = cosf(this->camera->transform.rotation[1]);
	vec3 right;
	vec3_mul_cross(right, this->camera->transform.up, forward);

	if (input_get_key(KEYCODE_LCONTROL)) {
		speed = slow_speed;
	}
	if (input_get_key(KEYCODE_LSHIFT)) {
		speed = fast_speed;
	}
	if (input_get_key(KEYCODE_W)) {
		vec3_add(this->camera->transform.position, this->camera->transform.position, (vec3) { forward[0] * speed, forward[1] * speed, forward[2] * speed });
	}
	if (input_get_key(KEYCODE_S)) {
		vec3_sub(this->camera->transform.position, this->camera->transform.position, (vec3) { forward[0] * speed, forward[1] * speed, forward[2] * speed });
	}
	if (input_get_key(KEYCODE_D)) {
		vec3_add(this->camera->transform.position, this->camera->transform.position, (vec3) { right[0] * speed, right[1] * speed, right[2] * speed });
	}
	if (input_get_key(KEYCODE_A)) {
		vec3_sub(this->camera->transform.position, this->camera->transform.position, (vec3) { right[0] * speed, right[1] * speed, right[2] * speed });
	}
	if (input_get_key(KEYCODE_E)) {
		vec3_add(this->camera->transform.position, this->camera->transform.position, (vec3) { this->camera->transform.up[0] * speed, this->camera->transform.up[1] * speed, this->camera->transform.up[2] * speed });
	}
	if (input_get_key(KEYCODE_Q)) {
		vec3_sub(this->camera->transform.position, this->camera->transform.position, (vec3) { this->camera->transform.up[0] * speed, this->camera->transform.up[1] * speed, this->camera->transform.up[2] * speed });
	}
	if (input_get_key(KEYCODE_RIGHT)) {
		this->camera->transform.rotation[1] += sensitivity;
	}
	if (input_get_key(KEYCODE_LEFT)) {
		this->camera->transform.rotation[1] -= sensitivity;
	}
	if (input_get_key(KEYCODE_UP)) {
		this->camera->transform.rotation[0] += sensitivity;
	}
	if (input_get_key(KEYCODE_DOWN)) {
		this->camera->transform.rotation[0] -= sensitivity;
	}

	KLOG("%f", this->camera->transform.rotation[0]);

	if (this->camera->transform.rotation[0] > 85) {
		this->camera->transform.rotation[0] = 85;
	}
	if (this->camera->transform.rotation[0] < -85) {
		this->camera->transform.rotation[0] = -85;
	}

	return TRUE;
}

b8 game_render(game_t * instance, f64 delta_time) {
	(void) instance;
	(void) delta_time;
	return TRUE;
}
