#include <defines.h>
#include <game.h>
#include <core/logger.h>
#include <core/input.h>
#include <core/event.h>
#include <core/file.h>
#include <core/mem.h>
#include <core/assert.h>
#include <containers/dynarray.h>
#include <containers/transform.h>
#include <containers/camera.h>
#include <math/linmath.h>
#include <math.h>

game_t * this = NULL;

file_t global_file;

static b8 on_key_press(u16 code, void * sender, void * listener, event_context_t ctx) {
	switch (ctx.data.u16[0]) {
		case KEYCODE_ESCAPE:
			input_set_cursor_state(CURSOR_STATE_NORMAL);
			break;
		case KEYCODE_F1:
			KLOG("%s", memory_get_usage_cstr());
			break;
		case KEYCODE_F12:
			DEBUG_BREAK();
			break;
		case KEYCODE_NUMPAD_5:
			this->camera->transform.rotation[0] = 0; this->camera->transform.rotation[1] = 0; this->camera->transform.rotation[2] = 0;
			break;
		case KEYCODE_NUMPAD_6:
			this->camera->transform.rotation[1] += 45;
			break;
		case KEYCODE_NUMPAD_4:
			this->camera->transform.rotation[1] += -45;
			break;
		case KEYCODE_NUMPAD_8:
			this->camera->transform.rotation[0] += 45;
			break;
		case KEYCODE_NUMPAD_2:
			this->camera->transform.rotation[0] += -45;
			break;
		case KEYCODE_NUMPAD_0:
			this->camera->transform.position[0] = 0; this->camera->transform.position[1] = 0; this->camera->transform.position[2] = 0;
			break;
		default:
			break;
	}

	return FALSE;
}

static b8 on_mouse_move(u16 code, void * sender, void * listener, event_context_t ctx) {
	return FALSE;
}

static b8 on_mouse_press(u16 code, void * sender, void * listener, event_context_t ctx) {
	input_set_cursor_state(CURSOR_STATE_GRABBED);

	return FALSE;
}

static b8 on_mouse_scroll(u16 code, void * sender, void * listener, event_context_t ctx) {
	return FALSE;
}

b8 game_initialize(game_t * instance) {
	this = instance;

	KLOG("%zu", sizeof(long long));

	file_open(&global_file, "assets/config.txt", FILE_READ);
	file_read(&global_file);
	file_stringify(&global_file);
	while (global_file.buffer[0] != 'q') {
		KLOG("config.txt:\n%s", global_file.buffer);
		while (!global_file.modified) {
			file_last_mod(&global_file);
		}
		global_file.modified = FALSE;
		file_read(&global_file);
		file_stringify(&global_file);
	}
	KLOG("config.txt:\n%s", global_file.buffer);
	file_close(&global_file);

	event_register(EVENT_CODE_KEY_PRESSED, (void *) game_initialize, on_key_press);
	event_register(EVENT_CODE_MOUSE_MOVED, (void *) game_initialize, on_mouse_move);
	event_register(EVENT_CODE_MOUSE_SCROLL, (void *) game_initialize, on_mouse_scroll);
	event_register(EVENT_CODE_MOUSE_BUTTON_PRESSED, (void *) game_initialize, on_mouse_press);

	return TRUE;
}

b8 game_update(game_t * instance, f64 delta_time) {
	(void) instance;
	(void) delta_time;

	float base_speed = 5.0f;
	float scroll_speed = 30.0f;
	float slow_speed = 1.0f;
	float fast_speed = 20.0f;
	float speed = base_speed;
	float base_sens = 150.0f;
	float slow_sens = 80.0f;
	float sensitivity = base_sens;
	float mouse_sens = 15.0f;

	vec3 forward;
	transform_forward_yaw(&this->camera->transform, forward);
	vec3 right;
	transform_right_yaw(&this->camera->transform, forward, right);

	if (input_get_key(KEYCODE_LCONTROL)) {
		speed = slow_speed;
		sensitivity = slow_sens;
	}
	if (input_get_key(KEYCODE_LSHIFT)) {
		speed = fast_speed;
	}
	if (input_get_key(KEYCODE_BACKSPACE)) {
		instance->running = FALSE;
		return TRUE;
	}
	if (input_get_key(KEYCODE_W)) {
		vec3_add(this->camera->transform.position, this->camera->transform.position, (vec3) { forward[0] * speed * delta_time, forward[1] * speed * delta_time, forward[2] * speed * delta_time });
	}
	if (input_get_key(KEYCODE_S)) {
		vec3_sub(this->camera->transform.position, this->camera->transform.position, (vec3) { forward[0] * speed * delta_time, forward[1] * speed * delta_time, forward[2] * speed * delta_time });
	}
	if (input_get_key(KEYCODE_D)) {
		vec3_add(this->camera->transform.position, this->camera->transform.position, (vec3) { right[0] * speed * delta_time, right[1] * speed * delta_time, right[2] * speed * delta_time });
	}
	if (input_get_key(KEYCODE_A)) {
		vec3_sub(this->camera->transform.position, this->camera->transform.position, (vec3) { right[0] * speed * delta_time, right[1] * speed * delta_time, right[2] * speed * delta_time });
	}
	if (input_get_key(KEYCODE_E)) {
		vec3_add(this->camera->transform.position, this->camera->transform.position, (vec3) { this->camera->transform.up[0] * speed * delta_time, this->camera->transform.up[1] * speed * delta_time, this->camera->transform.up[2] * speed * delta_time });
	}
	if (input_get_key(KEYCODE_Q)) {
		vec3_sub(this->camera->transform.position, this->camera->transform.position, (vec3) { this->camera->transform.up[0] * speed * delta_time, this->camera->transform.up[1] * speed * delta_time, this->camera->transform.up[2] * speed * delta_time });
	}
	if (input_get_key(KEYCODE_RIGHT)) {
		this->camera->transform.rotation[1] += sensitivity * delta_time;
	}
	if (input_get_key(KEYCODE_LEFT)) {
		this->camera->transform.rotation[1] -= sensitivity * delta_time;
	}
	if (input_get_key(KEYCODE_UP)) {
		this->camera->transform.rotation[0] += sensitivity * delta_time;
	}
	if (input_get_key(KEYCODE_DOWN)) {
		this->camera->transform.rotation[0] -= sensitivity * delta_time;
	}

	i16 mouse[2];
	input_get_mouse_delta(&mouse[0], &mouse[1]);
	this->camera->transform.rotation[0] += -mouse[1] * mouse_sens * delta_time;
	this->camera->transform.rotation[1] += mouse[0] * mouse_sens * delta_time;

	if (this->camera->transform.rotation[0] > 85) {
		this->camera->transform.rotation[0] = 85;
	}
	if (this->camera->transform.rotation[0] < -85) {
		this->camera->transform.rotation[0] = -85;
	}

	i8 scroll[2];
	input_get_mouse_scroll(&scroll[0], &scroll[1]);
	vec3_add(this->camera->transform.position, this->camera->transform.position, (vec3) { forward[0] * speed * delta_time * scroll_speed * scroll[1], forward[1] * speed * delta_time * scroll_speed * scroll[1], forward[2] * speed * delta_time * scroll_speed * scroll[1] });
	vec3_add(this->camera->transform.position, this->camera->transform.position, (vec3) { right[0] * speed * delta_time * scroll_speed * scroll[0], right[1] * speed * delta_time * scroll_speed * scroll[0], right[2] * speed * delta_time * scroll_speed * scroll[0] });

	return TRUE;
}

b8 game_render(game_t * instance, f64 delta_time) {
	(void) instance;
	(void) delta_time;
	return TRUE;
}
