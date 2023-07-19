#include <core/input.h>
#include <core/event.h>
#include <core/mem.h>
#include <core/logger.h>
#include <platform/platform.h>

typedef struct keyboardState {
	u8 keys[32];
} keyboard_state_t;

static void input_key_set(keyboard_state_t * state, keycodes_enum keycode, b8 pressed) {
	u8 index = keycode / 8;
	u8 bitshift = 1 << (keycode % 8);
	if (pressed) {
		state->keys[index] |= bitshift;
		return;
	}

	state->keys[index] &= ~bitshift;
}

static b8 input_key_get(keyboard_state_t * state, keycodes_enum keycode) {
	u8 index = keycode / 8;
	u8 bitshift = 1 << (keycode % 8);

	if ((state->keys[index] & bitshift) != 0) {
		return TRUE;
	}

	return FALSE;
}

typedef struct mouseState {
	i16 x;
	i16 y;
	u8 buttons;
	i8 scroll_x;
	i8 scroll_y;
} mouse_state_t;

static void input_mouse_set(mouse_state_t * state, mouse_buttons_enum mb, b8 pressed) {
	u8 bitshift = 1 << (mb % 8);

	if (pressed) {
		state->buttons |= bitshift;
		return;
	}

	state->buttons &= ~bitshift;
}

static b8 input_mouse_get(mouse_state_t * state, mouse_buttons_enum mb) {
	u8 bitshift = 1 << (mb % 8);

	if ((state->buttons & bitshift) != 0) {
		return TRUE;
	}

	return FALSE;
}

typedef struct inputState {
	keyboard_state_t kb_current;
	keyboard_state_t kb_prev;
	mouse_state_t m_current;
	mouse_state_t m_prev;
	cursor_state_enum cursor_state;
} input_state_t;

static b8 initialized = FALSE;
static input_state_t state;

static const char * input_key_strs[KEYCODE_MAX + 1];
const char * input_get_key_cstr(keycodes_enum key) {
	const char * str = input_key_strs[key];
	if (str == NULL) {
		return input_key_strs[KEYCODE_UNKNOWN];
	}

	return str;
}

static const char * input_mouse_button_strs[MOUSE_BUTTON_MAX + 1];
const char * input_get_mouse_button_cstr(mouse_buttons_enum mb) {
	const char * str = input_mouse_button_strs[mb];
	if (str == NULL) {
		return input_mouse_button_strs[MOUSE_BUTTON_UNKNOWN];
	}

	return str;
}

#define STRINGIFY(str) #str
#define DEFINE_KEYCODE_STRING(name) STRINGIFY(KEYCODE_##name)
#define DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(array, name, code) array[code] = DEFINE_KEYCODE_STRING(name)
#define DEFINE_MOUSE_BUTTON_STRING(name) STRINGIFY(MOUSE_BUTTON_##name)
#define DEFINE_MOUSE_BUTTON_STRING_ARRAY_ELEMENT(array, name, code) array[code] = DEFINE_MOUSE_BUTTON_STRING(name)

void input_init(void) {
	DEFINE_MOUSE_BUTTON_STRING_ARRAY_ELEMENT(input_mouse_button_strs, UNKNOWN, MOUSE_BUTTON_UNKNOWN);
	DEFINE_MOUSE_BUTTON_STRING_ARRAY_ELEMENT(input_mouse_button_strs, LMB, MOUSE_BUTTON_LMB);
	DEFINE_MOUSE_BUTTON_STRING_ARRAY_ELEMENT(input_mouse_button_strs, RMB, MOUSE_BUTTON_RMB);
	DEFINE_MOUSE_BUTTON_STRING_ARRAY_ELEMENT(input_mouse_button_strs, MMB, MOUSE_BUTTON_MMB);
	
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, UNKNOWN, 0x00);

	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, BACKSPACE, 0x08);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, ENTER, 0x0A);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, TAB, 0x09);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, CAPS, 0x15);

	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, PRTSCR, 0x1A);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, ESCAPE, 0x1B);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, INSERT, 0x1C);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, PAUSE, 0x1D);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, HOME, 0x1E);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, END, 0x1F);

	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, SPACE, 0x20);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, EXCLAMATION, 0x21);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, QUOTE, 0x22);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, HASH, 0x23);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, DOLLAR, 0x24);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, PERCENT, 0x25);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, AMPERSAND, 0x26);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, APOSTROPHE, 0x27);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, LPAREN, 0x28);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, RPAREN, 0x29);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, ASTERISK, 0x2A);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, PLUS, 0x2B);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, COMMA, 0x2C);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, MINUS, 0x2D);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, PERIOD, 0x2E);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, SLASH, 0x2F);

	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, 0, 0x30);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, 1, 0x31);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, 2, 0x32);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, 3, 0x33);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, 4, 0x34);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, 5, 0x35);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, 6, 0x36);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, 7, 0x37);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, 8, 0x38);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, 9, 0x39);

	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, COLON, 0x3A);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, SEMICOLON, 0x3B);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, LCHEVRON, 0x3C);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, EQUAL, 0x3D);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, RCHEVRON, 0x3E);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, QUESTION, 0x3F);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, ASPERAND, 0x40);

	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, A, 0x41);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, B, 0x42);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, C, 0x43);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, D, 0x44);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, E, 0x45);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F, 0x46);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, G, 0x47);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, H, 0x48);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, I, 0x49);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, J, 0x4A);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, K, 0x4B);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, L, 0x4C);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, M, 0x4D);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, N, 0x4E);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, O, 0x4F);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, P, 0x50);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, Q, 0x51);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, R, 0x52);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, S, 0x53);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, T, 0x54);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, U, 0x55);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, V, 0x56);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, W, 0x57);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, X, 0x58);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, Y, 0x59);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, Z, 0x5A);

	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, LBRACK, 0x5B);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, BACKSLASH, 0x5C);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, RBRACK, 0x5D);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, CARET, 0x5E);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, UNDERSCORE, 0x5F);

	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, GRAVE, 0x60);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F1, 0x61);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F2, 0x62);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F3, 0x63);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F4, 0x64);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F5, 0x65);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F6, 0x66);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F7, 0x67);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F8, 0x68);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F9, 0x69);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F10, 0x6A);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F11, 0x6B);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F12, 0x6C);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F13, 0x6D);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F14, 0x6E);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F15, 0x6F);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F16, 0x70);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F17, 0x71);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F18, 0x72);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F19, 0x73);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F20, 0x74);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F21, 0x75);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F22, 0x76);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F23, 0x77);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, F24, 0x78);

	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, PGUP, 0x79);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, PGDN, 0x7A);
	
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, LBRACE, 0x7B);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, PIPE, 0x7C);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, RBRACE, 0x7D);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, TILDE, 0x7E);

	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, DEL, 0x7F);

	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_0, 0x80);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_1, 0x81);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_2, 0x82);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_3, 0x83);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_4, 0x84);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_5, 0x85);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_6, 0x86);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_7, 0x87);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_8, 0x88);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_9, 0x89);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_MULTIPLY, 0x8A);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_ADD, 0x8B);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_SEPARATOR, 0x8C);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_SUBTRACT, 0x8D);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_DECIMAL, 0x8E);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_DIVIDE, 0x8F);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_ENTER, 0x90);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMPAD_EQUAL, 0x91);

	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, NUMLOCK, 0x92);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, SCROLL_LOCK, 0x93);

	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, RIGHT, 0x94);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, LEFT, 0x95);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, UP, 0x96);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, DOWN, 0x97);

	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, LSHIFT, 0xA0);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, RSHIFT, 0xA1);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, LCONTROL, 0xA2);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, RCONTROL, 0xA3);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, LALT, 0xA4);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, RALT, 0xA5);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, LSUPER, 0xA6);
	DEFINE_KEYCODE_STRING_ARRAY_ELEMENT(input_key_strs, RSUPER, 0xA7);
	
	kmemzero(&state, sizeof(input_state_t));
	initialized = TRUE;
}

void input_deinit(void) {
	initialized = FALSE;
}

void input_update(f64 delta_time) {
	if (!initialized) {
		KWARN("[input_update(delta_time)]");
		KWARN("calling input_update() before input subsystem is initialized");

		return;
	}

	kmemcpy(&state.kb_prev, &state.kb_current, sizeof(keyboard_state_t));
	kmemcpy(&state.m_prev, &state.m_current, sizeof(mouse_state_t));

	state.m_current.scroll_x = 0;
	state.m_current.scroll_y = 0;
}

void input_process_key(keycodes_enum key, b8 pressed) {
	if (input_key_get(&state.kb_current, key) != pressed) {
		input_key_set(&state.kb_current, key, pressed);
		//KDEBUG("key %s was %s", input_key_strs[key], pressed ? "pressed" : "released");

		event_context_t ctx;
		ctx.data.u16[0] = key;
		event_fire(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, (void *) input_process_key, ctx);
	}
}

void input_process_mouse_button(mouse_buttons_enum mb, b8 pressed) {
	if (input_mouse_get(&state.m_current, mb) != pressed) {
		input_mouse_set(&state.m_current, mb, pressed);
		//KDEBUG("mouse button %u %s", mb, pressed ? "pressed" : "released");

		event_context_t ctx;
		ctx.data.u8[0] = mb;
		event_fire(pressed ? EVENT_CODE_MOUSE_BUTTON_PRESSED : EVENT_CODE_MOUSE_BUTTON_RELEASED, (void *) input_process_mouse_button, ctx);
	}
}

void input_process_mouse_move(i32 x, i32 y) {
	if (state.m_current.x != x || state.m_current.y != y) {
		//KDEBUG("mouse pos: (%i, %i)", x, y);
		state.m_current.x = x;
		state.m_current.y = y;

		event_context_t ctx;
		ctx.data.i32[0] = x;
		ctx.data.i32[1] = y;
		event_fire(EVENT_CODE_MOUSE_MOVED, (void *) input_process_mouse_move, ctx);
	}
}

void input_process_mouse_scroll(i8 dx, i8 dy) {
	state.m_current.scroll_x = -dx;
	state.m_current.scroll_y = dy;

	event_context_t ctx;
	ctx.data.i8[0] = -dx;
	ctx.data.i8[1] = dy;
	event_fire(EVENT_CODE_MOUSE_SCROLL, (void *) input_process_mouse_scroll, ctx);
}

void input_set_cursor_state(cursor_state_enum cursor_state) {
	state.cursor_state = cursor_state;
	platform_set_cursor(cursor_state);
}

cursor_state_enum input_get_cursor_state(void) {
	return state.cursor_state;
}

b8 input_get_key(keycodes_enum key) {
	if (!initialized) {
		return FALSE;
	}

	return input_key_get(&state.kb_current, key);
}

b8 input_get_key_down(keycodes_enum key) {
	if (!initialized) {
		return FALSE;
	}

	if (input_key_get(&state.kb_prev, key) == TRUE) {
		return FALSE;
	}

	return input_key_get(&state.kb_current, key);
}

b8 input_get_key_up(keycodes_enum key) {
	if (!initialized) {
		return TRUE;
	}

	if (input_key_get(&state.kb_prev, key) == FALSE) {
		return FALSE;
	}

	return input_key_get(&state.kb_current, key);
}

b8 input_get_mouse_button(mouse_buttons_enum mb) {
	if (!initialized) {
		return FALSE;
	}

	return input_mouse_get(&state.m_current, mb);
}

b8 input_get_mouse_button_down(mouse_buttons_enum mb) {
	if (!initialized) {
		return FALSE;
	}

	if (input_mouse_get(&state.m_prev, mb) == TRUE) {
		return FALSE;
	}

	return input_mouse_get(&state.m_current, mb);
}

b8 input_get_mouse_button_up(mouse_buttons_enum mb) {
	if (!initialized) {
		return TRUE;
	}

	if (input_mouse_get(&state.m_prev, mb) == FALSE) {
		return FALSE;
	}

	return input_mouse_get(&state.m_current, mb);
}

void input_get_mouse_pos(i32 * x, i32 * y) {
	if (!initialized) {
		*x = 0; *y = 0;
		return;
	}

	*x = state.m_current.x;
	*y = state.m_current.y;
}

void input_get_mouse_delta(i16 * x, i16 * y) {
	if (!initialized) {
		*x = 0; *y = 0;
		return;
	}

	*x = state.m_current.x - state.m_prev.x;
	*y = state.m_current.y - state.m_prev.y;
}

void input_get_mouse_scroll(i8 * dx, i8 * dy) {
	if (!initialized) {
		*dx = 0; *dy = 0;
		return;
	}

	*dx = state.m_current.scroll_x;
	*dy = state.m_current.scroll_y;
}