#include <platform/platform.h>
#include <defines.h>

/*
#if KPLATFORM_[PLATFORM]

#include <GLFW/glfw3.h>
#include <core/logger.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void glfw_error_handler(int error, const char * desc);
static void glfw_key_handler(GLFWwindow * window, int key, int scancode, int action, int mods);
static void glfw_mouse_pos_handler(GLFWwindow * window, f64 x, f64 y);
static void glfw_mouse_button_handler(GLFWwindow * window, int button, int action, int mods);
static void glfw_mouse_scroll_handler(GLFWwindow * window, double dx, double dy);
static void glfw_window_close_handler(GLFWwindow * window);

typedef struct internalState {
	GLFWwindow * glfw_win;
	b8 running;
} internal_state_t;

static internal_state_t * current_state;

b8 platform_startup(
	platform_state_t * platform_state,
	const char * app_name,
	i32 x, i32 y, i32 w, i32 h
) {
	platform_state->internal_state = malloc(sizeof(internal_state_t));
	internal_state_t * state = (internal_state_t *) platform_state->internal_state;
	current_state = state;

	glfwSetErrorCallback(glfw_error_handler);

	if (glfwInit() != GLFW_TRUE) {
		KFATAL("GLFW failed to initialize");
		return FALSE;
	}

	if (!glfwVulkanSupported()) {
		KFATAL("GLFW does not support vulkan");
		glfwTerminate();
		return FALSE;
	}

	state->glfw_win = glfwCreateWindow(w, h, app_name, NULL, NULL);
	if (state->glfw_win == NULL) {
		KFATAL("GLFW failed to create window");
		glfwTerminate();
		return FALSE;
	}

	glfwSetWindowPos(state->glfw_win, x, y);
	glfwSetKeyCallback(state->glfw_win, glfw_key_handler);
	glfwSetCursorPosCallback(state->glfw_win, glfw_mouse_pos_handler);
	glfwSetMouseButtonCallback(state->glfw_win, glfw_mouse_button_handler);
	glfwSetScrollCallback(state->glfw_win, glfw_mouse_scroll_handler);
	glfwSetWindowCloseCallback(state->glfw_win, glfw_window_close_handler);

	state->running = TRUE;

	return TRUE;
}

void platform_shutdown(platform_state_t * platform_state) {
	internal_state_t * state = (internal_state_t *) platform_state->internal_state;

	glfwDestroyWindow(state->glfw_win);
	glfwTerminate();

	free(state);
}

b8 platform_pump_messages(platform_state_t * platform_state) {
	internal_state_t * state = (internal_state_t *) platform_state->internal_state;

	if (!state->running || state->glfw_win == NULL) {
		return FALSE;
	}

	glfwPollEvents();
	return TRUE;
}

void * platform_malloc(u64 size, b8 aligned) {
	return malloc(size);
}

void platform_free(void * ptr, b8 aligned) {
	free(ptr);
}

void * platform_memset(void * dst, i32 value, u64 size) {
	return memset(dst, value, size);
}

void * platform_memcpy(void * dst, const void * src, u64 size) {
	return memcpy(dst, src, size);
}

void * platform_memzero(void * dst, u64 size) {
	return memset(dst, 0, size);
}

void platform_console_write(const char * message, u8 color) {
	printf("%s", message);
}

void platform_console_write_error(const char * message, u8 color) {
	fprintf(stderr, "%s", message);
}

f64 platform_get_absolute_time(void) {
	// implement time stuff
}

void platform_sleep(u64 ms) {
	// implement sleeping
}

static void glfw_error_handler(int error, const char * desc) {
	KERROR("GLFW error: %i, \"%s\"", error, desc);
}

#define KEYCODE_MAP_GLFW_KEY(key, keycode) case GLFW_KEY_##key: return KEYCODE_##keycode;
#define KEYCODE_MAP_GLFW_KEY_SAME_NAME(key) KEYCODE_MAP_GLFW_KEY(key, key)

static keycodes_enum glfw_key_map(int key) {
	switch (key) {
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(UNKNOWN);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(SPACE);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(APOSTROPHE);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(COMMA);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(MINUS);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(PERIOD);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(SLASH);

		KEYCODE_MAP_GLFW_KEY_SAME_NAME(0);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(1);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(2);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(3);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(4);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(5);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(6);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(7);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(8);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(9);

		KEYCODE_MAP_GLFW_KEY_SAME_NAME(SEMICOLON);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(EQUAL);

		KEYCODE_MAP_GLFW_KEY_SAME_NAME(A);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(B);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(C);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(D);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(E);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(G);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(H);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(I);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(J);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(K);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(L);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(M);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(N);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(O);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(P);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(Q);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(R);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(S);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(T);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(U);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(V);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(W);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(X);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(Y);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(Z);

		KEYCODE_MAP_GLFW_KEY(LEFT_BRACKET, LBRACK);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(BACKSLASH);
		KEYCODE_MAP_GLFW_KEY(RIGHT_BRACKET, RBRACK);
		KEYCODE_MAP_GLFW_KEY(GRAVE_ACCENT, GRAVE);

		KEYCODE_MAP_GLFW_KEY(WORLD_1, UNKNOWN);
		KEYCODE_MAP_GLFW_KEY(WORLD_2, UNKNOWN);

		KEYCODE_MAP_GLFW_KEY_SAME_NAME(ESCAPE);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(ENTER);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(TAB);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(BACKSPACE);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(INSERT);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(DELETE);

		KEYCODE_MAP_GLFW_KEY_SAME_NAME(RIGHT);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(LEFT);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(UP);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(DOWN);

		KEYCODE_MAP_GLFW_KEY(PAGE_UP, PGUP);
		KEYCODE_MAP_GLFW_KEY(PAGE_DOWN, PGDN);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(HOME);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(END);

		KEYCODE_MAP_GLFW_KEY(CAPS_LOCK, CAPS);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(SCROLL_LOCK);
		KEYCODE_MAP_GLFW_KEY(PRINT_SCREEN, PRTSCR);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(PAUSE);

		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F1);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F2);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F3);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F4);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F5);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F6);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F7);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F8);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F9);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F10);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F11);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F12);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F13);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F14);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F15);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F16);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F17);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F18);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F19);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F20);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F21);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F22);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F23);
		KEYCODE_MAP_GLFW_KEY_SAME_NAME(F24);

		KEYCODE_MAP_GLFW_KEY(KP_0, NUMPAD_0);
		KEYCODE_MAP_GLFW_KEY(KP_1, NUMPAD_1);
		KEYCODE_MAP_GLFW_KEY(KP_2, NUMPAD_2);
		KEYCODE_MAP_GLFW_KEY(KP_3, NUMPAD_3);
		KEYCODE_MAP_GLFW_KEY(KP_4, NUMPAD_4);
		KEYCODE_MAP_GLFW_KEY(KP_5, NUMPAD_5);
		KEYCODE_MAP_GLFW_KEY(KP_6, NUMPAD_6);
		KEYCODE_MAP_GLFW_KEY(KP_7, NUMPAD_7);
		KEYCODE_MAP_GLFW_KEY(KP_8, NUMPAD_8);
		KEYCODE_MAP_GLFW_KEY(KP_9, NUMPAD_9);
		KEYCODE_MAP_GLFW_KEY(KP_DECIMAL, NUMPAD_DECIMAL);
		KEYCODE_MAP_GLFW_KEY(KP_DIVIDE, NUMPAD_DIVIDE);
		KEYCODE_MAP_GLFW_KEY(KP_MULTIPLY, NUMPAD_MULTIPLY);
		KEYCODE_MAP_GLFW_KEY(KP_SUBTRACT, NUMPAD_SUBTRACT);
		KEYCODE_MAP_GLFW_KEY(KP_ADD, NUMPAD_ADD);
		KEYCODE_MAP_GLFW_KEY(KP_ENTER, NUMPAD_ENTER);
		KEYCODE_MAP_GLFW_KEY(KP_EQUAL, NUMPAD_EQUAL);

		KEYCODE_MAP_GLFW_KEY(LEFT_SHIFT, LSHIFT);
		KEYCODE_MAP_GLFW_KEY(LEFT_CONTROL, LCONTROL);
		KEYCODE_MAP_GLFW_KEY(LEFT_ALT, LALT);
		KEYCODE_MAP_GLFW_KEY(LEFT_SUPER, LSUPER);
		KEYCODE_MAP_GLFW_KEY(RIGHT_SHIFT, RSHIFT);
		KEYCODE_MAP_GLFW_KEY(RIGHT_CONTROL, RCONTROL);
		KEYCODE_MAP_GLFW_KEY(RIGHT_ALT, RALT);
		KEYCODE_MAP_GLFW_KEY(RIGHT_SUPER, RSUPER);

		default: return KEYCODE_UNKNOWN;
	}
}

static void glfw_key_handler(GLFWwindow * window, int key, int scancode, int action, int mods) {
	//KLOG("window %p: key %d %s, %d, %d, %d pressed", (void *) window, key, glfwGetKeyName(key, 0), scancode, action, mods);
	input_process_key(glfw_key_map(key), action ? TRUE : FALSE);
}

static void glfw_mouse_pos_handler(GLFWwindow * window, f64 x, f64 y) {
	//KLOG("window %p: mouse moved (%lf, %lf)", (void *) window, x, y);
	input_process_mouse_move((i32) x, (i32) y);
}

#define MOUSE_BUTTON_MAP_GLFW_MOUSE_BUTTON(button, mb) case GLFW_MOUSE_BUTTON_##button: return MOUSE_BUTTON_##mb;

static mouse_buttons_enum glfw_mouse_button_map(int button) {
	switch (button) {
		MOUSE_BUTTON_MAP_GLFW_MOUSE_BUTTON(1, 0);
		MOUSE_BUTTON_MAP_GLFW_MOUSE_BUTTON(2, 1);
		MOUSE_BUTTON_MAP_GLFW_MOUSE_BUTTON(3, 2);
		default: return MOUSE_BUTTON_UNKNOWN;
	}
}

static void glfw_mouse_button_handler(GLFWwindow * window, int button, int action, int mods) {
	//KLOG("window %p: mouse button %d, %d, %d pressed", (void *) window, button, action, mods);
	input_process_mouse_button(glfw_mouse_button_map(button), action ? TRUE : FALSE);
}

static void glfw_mouse_scroll_handler(GLFWwindow * window, double dx, double dy) {
	//KLOG("window %p: mouse scroll %lf %lf", (void *) window, dx, dy);
	input_process_mouse_scroll(dx, dy);
}

static void glfw_window_close_handler(GLFWwindow * window) {
	//KLOG("window %p is being closed", (void *) window);
	glfwDestroyWindow(window);
	current_state->glfw_win = NULL;
}

#endif
*/
