#ifndef KENGINE_CORE_INPUT_H
#define KENGINE_CORE_INPUT_H

#include <defines.h>

typedef enum mouseButtonsEnum {
	MOUSE_BUTTON_UNKNOWN,
	MOUSE_BUTTON_0 = 1,
	MOUSE_BUTTON_LMB = MOUSE_BUTTON_0,
	MOUSE_BUTTON_1 = 2,
	MOUSE_BUTTON_RMB = MOUSE_BUTTON_1,
	MOUSE_BUTTON_2 = 3,
	MOUSE_BUTTON_MMB = MOUSE_BUTTON_2,
	MOUSE_BUTTON_MAX = MOUSE_BUTTON_MMB,
} mouse_buttons_enum;

#define DEFINE_KEYCODE(name, code) KEYCODE_##name = code

typedef enum keycodesEnum {
	DEFINE_KEYCODE(UNKNOWN, 0x00),

	DEFINE_KEYCODE(BACKSPACE, 0x08),
	DEFINE_KEYCODE(ENTER, 0x0A),
	DEFINE_KEYCODE(TAB, 0x09),
	DEFINE_KEYCODE(CAPS, 0x15),

	DEFINE_KEYCODE(PRTSCR, 0x1A),
	DEFINE_KEYCODE(ESCAPE, 0x1B),
	DEFINE_KEYCODE(INSERT, 0x1C),
	DEFINE_KEYCODE(PAUSE, 0x1D),
	DEFINE_KEYCODE(HOME, 0x1E),
	DEFINE_KEYCODE(END, 0x1F),

	DEFINE_KEYCODE(SPACE, 0x20),
	DEFINE_KEYCODE(EXCLAMATION, 0x21),
	DEFINE_KEYCODE(QUOTE, 0x22),
	DEFINE_KEYCODE(HASH, 0x23),
	DEFINE_KEYCODE(DOLLAR, 0x24),
	DEFINE_KEYCODE(PERCENT, 0x25),
	DEFINE_KEYCODE(AMPERSAND, 0x26),
	DEFINE_KEYCODE(APOSTROPHE, 0x27),
	DEFINE_KEYCODE(LPAREN, 0x28),
	DEFINE_KEYCODE(RPAREN, 0x29),
	DEFINE_KEYCODE(ASTERISK, 0x2A),
	DEFINE_KEYCODE(PLUS, 0x2B),
	DEFINE_KEYCODE(COMMA, 0x2C),
	DEFINE_KEYCODE(MINUS, 0x2D),
	DEFINE_KEYCODE(PERIOD, 0x2E),
	DEFINE_KEYCODE(SLASH, 0x2F),

	DEFINE_KEYCODE(0, 0x30),
	DEFINE_KEYCODE(1, 0x31),
	DEFINE_KEYCODE(2, 0x32),
	DEFINE_KEYCODE(3, 0x33),
	DEFINE_KEYCODE(4, 0x34),
	DEFINE_KEYCODE(5, 0x35),
	DEFINE_KEYCODE(6, 0x36),
	DEFINE_KEYCODE(7, 0x37),
	DEFINE_KEYCODE(8, 0x38),
	DEFINE_KEYCODE(9, 0x39),

	DEFINE_KEYCODE(COLON, 0x3A),
	DEFINE_KEYCODE(SEMICOLON, 0x3B),
	DEFINE_KEYCODE(LCHEVRON, 0x3C),
	DEFINE_KEYCODE(EQUAL, 0x3D),
	DEFINE_KEYCODE(RCHEVRON, 0x3E),
	DEFINE_KEYCODE(QUESTION, 0x3F),
	DEFINE_KEYCODE(ASPERAND, 0x40),

	DEFINE_KEYCODE(A, 0x41),
	DEFINE_KEYCODE(B, 0x42),
	DEFINE_KEYCODE(C, 0x43),
	DEFINE_KEYCODE(D, 0x44),
	DEFINE_KEYCODE(E, 0x45),
	DEFINE_KEYCODE(F, 0x46),
	DEFINE_KEYCODE(G, 0x47),
	DEFINE_KEYCODE(H, 0x48),
	DEFINE_KEYCODE(I, 0x49),
	DEFINE_KEYCODE(J, 0x4A),
	DEFINE_KEYCODE(K, 0x4B),
	DEFINE_KEYCODE(L, 0x4C),
	DEFINE_KEYCODE(M, 0x4D),
	DEFINE_KEYCODE(N, 0x4E),
	DEFINE_KEYCODE(O, 0x4F),
	DEFINE_KEYCODE(P, 0x50),
	DEFINE_KEYCODE(Q, 0x51),
	DEFINE_KEYCODE(R, 0x52),
	DEFINE_KEYCODE(S, 0x53),
	DEFINE_KEYCODE(T, 0x54),
	DEFINE_KEYCODE(U, 0x55),
	DEFINE_KEYCODE(V, 0x56),
	DEFINE_KEYCODE(W, 0x57),
	DEFINE_KEYCODE(X, 0x58),
	DEFINE_KEYCODE(Y, 0x59),
	DEFINE_KEYCODE(Z, 0x5A),

	DEFINE_KEYCODE(LBRACK, 0x5B),
	DEFINE_KEYCODE(BACKSLASH, 0x5C),
	DEFINE_KEYCODE(RBRACK, 0x5D),
	DEFINE_KEYCODE(CARET, 0x5E),
	DEFINE_KEYCODE(UNDERSCORE, 0x5F),

	DEFINE_KEYCODE(GRAVE, 0x60),
	DEFINE_KEYCODE(F1, 0x61),
	DEFINE_KEYCODE(F2, 0x62),
	DEFINE_KEYCODE(F3, 0x63),
	DEFINE_KEYCODE(F4, 0x64),
	DEFINE_KEYCODE(F5, 0x65),
	DEFINE_KEYCODE(F6, 0x66),
	DEFINE_KEYCODE(F7, 0x67),
	DEFINE_KEYCODE(F8, 0x68),
	DEFINE_KEYCODE(F9, 0x69),
	DEFINE_KEYCODE(F10, 0x6A),
	DEFINE_KEYCODE(F11, 0x6B),
	DEFINE_KEYCODE(F12, 0x6C),
	DEFINE_KEYCODE(F13, 0x6D),
	DEFINE_KEYCODE(F14, 0x6E),
	DEFINE_KEYCODE(F15, 0x6F),
	DEFINE_KEYCODE(F16, 0x70),
	DEFINE_KEYCODE(F17, 0x71),
	DEFINE_KEYCODE(F18, 0x72),
	DEFINE_KEYCODE(F19, 0x73),
	DEFINE_KEYCODE(F20, 0x74),
	DEFINE_KEYCODE(F21, 0x75),
	DEFINE_KEYCODE(F22, 0x76),
	DEFINE_KEYCODE(F23, 0x77),
	DEFINE_KEYCODE(F24, 0x78),

	DEFINE_KEYCODE(PGUP, 0x79),
	DEFINE_KEYCODE(PGDN, 0x7A),
	
	DEFINE_KEYCODE(LBRACE, 0x7B),
	DEFINE_KEYCODE(PIPE, 0x7C),
	DEFINE_KEYCODE(RBRACE, 0x7D),
	DEFINE_KEYCODE(TILDE, 0x7E),

	DEFINE_KEYCODE(DEL, 0x7F),

	DEFINE_KEYCODE(NUMPAD_0, 0x80),
	DEFINE_KEYCODE(NUMPAD_1, 0x81),
	DEFINE_KEYCODE(NUMPAD_2, 0x82),
	DEFINE_KEYCODE(NUMPAD_3, 0x83),
	DEFINE_KEYCODE(NUMPAD_4, 0x84),
	DEFINE_KEYCODE(NUMPAD_5, 0x85),
	DEFINE_KEYCODE(NUMPAD_6, 0x86),
	DEFINE_KEYCODE(NUMPAD_7, 0x87),
	DEFINE_KEYCODE(NUMPAD_8, 0x88),
	DEFINE_KEYCODE(NUMPAD_9, 0x89),
	DEFINE_KEYCODE(NUMPAD_MULTIPLY, 0x8A),
	DEFINE_KEYCODE(NUMPAD_ADD, 0x8B),
	DEFINE_KEYCODE(NUMPAD_SEPARATOR, 0x8C),
	DEFINE_KEYCODE(NUMPAD_SUBTRACT, 0x8D),
	DEFINE_KEYCODE(NUMPAD_DECIMAL, 0x8E),
	DEFINE_KEYCODE(NUMPAD_DIVIDE, 0x8F),
	DEFINE_KEYCODE(NUMPAD_ENTER, 0x90),
	DEFINE_KEYCODE(NUMPAD_EQUAL, 0x91),

	DEFINE_KEYCODE(NUMLOCK, 0x92),
	DEFINE_KEYCODE(SCROLL_LOCK, 0x93),

	DEFINE_KEYCODE(RIGHT, 0x94),
	DEFINE_KEYCODE(LEFT, 0x95),
	DEFINE_KEYCODE(UP, 0x96),
	DEFINE_KEYCODE(DOWN, 0x97),

	DEFINE_KEYCODE(LSHIFT, 0xA0),
	DEFINE_KEYCODE(RSHIFT, 0xA1),
	DEFINE_KEYCODE(LCONTROL, 0xA2),
	DEFINE_KEYCODE(RCONTROL, 0xA3),
	DEFINE_KEYCODE(LALT, 0xA4),
	DEFINE_KEYCODE(RALT, 0xA5),
	DEFINE_KEYCODE(LSUPER, 0xA6),
	DEFINE_KEYCODE(RSUPER, 0xA7),

    DEFINE_KEYCODE(MAX, KEYCODE_RSUPER),
} keycodes_enum;

typedef enum cursorStateEnum {
	CURSOR_STATE_NORMAL = 0,
	CURSOR_STATE_HIDDEN = 1,
	CURSOR_STATE_GRABBED = 2,
} cursor_state_enum;

typedef u8 input_status_t;

void input_init(void);
void input_deinit(void);
void input_update(f64 delta_time);

b8 KAPI input_get_key(keycodes_enum key);
b8 KAPI input_get_key_down(keycodes_enum key);
b8 KAPI input_get_key_up(keycodes_enum key);

const char KAPI * input_get_key_cstr(keycodes_enum key);
const char KAPI * input_get_mouse_button_cstr(mouse_buttons_enum mb);

void input_process_key(keycodes_enum key, b8 pressed);

b8 KAPI input_get_mouse_button(mouse_buttons_enum mb);
b8 KAPI input_get_mouse_button_down(mouse_buttons_enum mb);
b8 KAPI input_get_mouse_button_up(mouse_buttons_enum mb);
void KAPI input_get_mouse_pos(i32 * x, i32 * y);
void KAPI input_get_mouse_delta(i16 * x, i16 * y);
void KAPI input_get_mouse_scroll(i8 * dx, i8 * dy);

void KAPI input_set_cursor_state(cursor_state_enum state);
cursor_state_enum KAPI input_get_cursor_state(void);

void input_process_mouse_button(mouse_buttons_enum mb, b8 pressed);
void input_process_mouse_move(i32 x, i32 y);
void input_process_mouse_scroll(i8 dx, i8 dy);

#endif
