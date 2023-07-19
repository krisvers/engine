#include <platform/platform.h>
#include <defines.h>

#if KPLATFORM_LINUX

#ifdef KPLATFORM_VULKAN
#include <renderer/vulkan/vulkan_types.h>
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#endif
#ifdef KPLATFORM_OPENGL
#include <renderer/opengl/opengl_types.h>
#include <glad/glad.h>
#endif
#include <renderer/frontend.h>
#include <GLFW/glfw3.h>
#include <core/input.h>
#include <core/logger.h>
#include <containers/dynarray.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <sys/time.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

static void glfw_error_handler(int error, const char * desc);
static void glfw_key_handler(GLFWwindow * window, int key, int scancode, int action, int mods);
static void glfw_mouse_pos_handler(GLFWwindow * window, f64 x, f64 y);
static void glfw_mouse_button_handler(GLFWwindow * window, int button, int action, int mods);
static void glfw_mouse_scroll_handler(GLFWwindow * window, double dx, double dy);
static void glfw_window_close_handler(GLFWwindow * window);
static void glfw_window_resize_handler(GLFWwindow * window, int w, int h);

typedef struct internalState {
	GLFWwindow * glfw_win;
#ifdef KPLATFORM_VULKAN
	VkSurfaceKHR surface;
#endif
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

	#ifdef KPLATFORM_VULKAN
	if (!glfwVulkanSupported()) {
		KFATAL("GLFW does not support vulkan");
		glfwTerminate();
		return FALSE;
	}
	#endif
	#ifndef KPLATFORM_OPENGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	#else
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	state->glfw_win = glfwCreateWindow(w, h, app_name, NULL, NULL);
	if (state->glfw_win == NULL) {
		KFATAL("GLFW failed to create window");
		glfwTerminate();
		return FALSE;
	}

	glfwMakeContextCurrent(state->glfw_win);
	#ifdef KPLATFORM_OPENGL
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		KFATAL("failed to load OpenGL with GLAD");
		glfwDestroyWindow(state->glfw_win);
		glfwTerminate();
		return FALSE;
	}

	glViewport(0, 0, w, h);
	KLOG("Highest supported OpenGL version: %s", glGetString(GL_VERSION));
	#endif
	glfwSetWindowPos(state->glfw_win, x, y);
	glfwSetKeyCallback(state->glfw_win, glfw_key_handler);
	glfwSetCursorPosCallback(state->glfw_win, glfw_mouse_pos_handler);
	glfwSetMouseButtonCallback(state->glfw_win, glfw_mouse_button_handler);
	glfwSetScrollCallback(state->glfw_win, glfw_mouse_scroll_handler);
	glfwSetWindowCloseCallback(state->glfw_win, glfw_window_close_handler);
	glfwSetFramebufferSizeCallback(state->glfw_win, glfw_window_resize_handler);

	glfwSetTime(0);

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

	if (state->glfw_win == NULL) {
		return FALSE;
	}
	glfwPollEvents();
	if (state->glfw_win == NULL) {
		return FALSE;
	}
	return TRUE;
}

void platform_swap_buffers(platform_state_t * platform_state) {
	internal_state_t * state = (internal_state_t *) platform_state->internal_state;
	if (state->glfw_win == NULL) {
		return;
	}
	glfwSwapBuffers(state->glfw_win);
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
	static const char * escape_colors[6] = { "0;41", "1;31", "1;33", "1;32", "1;34", "1;36" };

	if (color >= LOG_LEVEL_TRACE) {
		printf("%s", message);
		return;
	}
	printf("\033[%sm%s\033[0m", escape_colors[color], message);
}

void platform_console_write_error(const char * message, u8 color) {
	static const char * escape_colors[6] = { "0;41", "1;31", "1;33", "1;32", "1;34", "1;36" };

	if (color >= LOG_LEVEL_TRACE) {
		printf("%s", message);
		return;
	}
	fprintf(stderr, "\033[%sm%s\033[0m", escape_colors[color], message);
}

f64 platform_get_absolute_time(void) {
	return glfwGetTime();
}

void platform_sleep(u64 ms) {
	struct timeval tv;
	tv.tv_sec = ms / 1000;
	tv.tv_usec = (ms % 1000) * 1000;
	select(0, NULL, NULL, NULL, &tv);
}

void platform_set_cursor(u8 value) {
	switch (value) {
		default:
		case 0:
			glfwSetInputMode(current_state->glfw_win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			return;
		case 1:
			glfwSetInputMode(current_state->glfw_win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			return;
		case 2:
			glfwSetInputMode(current_state->glfw_win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			return;
	}
}

static const char * file_operation_cstrs[] = {
	"r",
	"w",
	"rb",
	"wb",
	"a",
	"ab",
};

file_desc_t platform_file_open(char * filename, u8 op) {
	FILE * fp = fopen(filename, file_operation_cstrs[op]);
	if (fp == NULL) {
		KERROR("[platform_file_open(filename, op)]");
		KERROR("failed to open file at %s", filename);
		return NULL;
	}

	return fp;
}

void platform_file_close(file_desc_t fp) {
	if (fp == NULL) {
		KERROR("[platform_file_close(fp)]");
		KERROR("given file descriptor does not describe a file");
		return;
	}

	fclose(fp);
}

void platform_file_read(file_desc_t fp, u64 length, u8 * buffer) {
	fseek(fp, 0L, SEEK_SET);
	u64 res = fread(buffer, length, 1, fp);
	if (res != 1 && res != length) {
		KERROR("[platform_file_read(fp, length, buffer)]");
		KERROR("failure whilst reading file");
		return;
	}
}

void platform_file_write(file_desc_t fp, u64 length, u8 * buffer) {
	fseek(fp, 0L, SEEK_SET);
	if (fwrite(buffer, length, 1, fp) != 1) {
		KERROR("[platform_file_write(fp, length, buffer)]");
		KERROR("failure whilst writing to file");
		return;
	}
}

u64 platform_file_length(file_desc_t fp) {
	if (fp == NULL) {
		KERROR("[platform_file_length(fp)]");
		KERROR("given file descriptor does not describe a file");
		return 0;
	}

	fseek(fp, 0L, SEEK_END);
	u64 len = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	return len;
}

f64 platform_file_last_modification(file_desc_t fp, char * path) {
	struct stat st;

	int fd = fileno(fp);
	if (fd == -1) {
		goto platform_file_last_modification_path_fallback;
	}

	int res = fstat(fd, &st);
	if (res == -1) {
		goto platform_file_last_modification_path_fallback;
	}
	goto platform_file_last_modification_path_fallback_end;

platform_file_last_modification_path_fallback:
	res = stat(path, &st);
	if (res == -1) {
		KERROR("[platform_file_last_modification(fp, path)]");
		KERROR("failed to obtain file status from fp and path %s", path);
		return -1;
	}
platform_file_last_modification_path_fallback_end:;

	f64 mod = st.st_mtim.tv_nsec / 1000.0f + st.st_mtim.tv_sec * 1000;
	return mod;
}

#ifdef KPLATFORM_VULKAN
void platform_get_required_extension_names(dynarray_t ** array) {
	u32 glfw_max_extensions;
	const char ** glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_max_extensions);
	dynarray_merge_array(*array, glfw_extensions, glfw_max_extensions);
}

b8 platform_create_vulkan_surface(platform_state_t * pstate, vulkan_context_t * context) {
	internal_state_t * state = (internal_state_t *) pstate->internal_state;
	VkResult ret = glfwCreateWindowSurface(context->instance, state->glfw_win, context->allocator, &state->surface);
	if (ret != VK_SUCCESS) {
		KFATAL("[platform_create_vulkan_surface(pstate, context)]");
		KFATAL("failed to create window surface using GLFW");
		return FALSE;
	}
	context->surface = state->surface;
	return TRUE;
}
#endif

/* glfw specific stuff */
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
		KEYCODE_MAP_GLFW_KEY(DELETE, DEL);

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

static void glfw_window_resize_handler(GLFWwindow * window, int w, int h) {
	renderer_on_resize(w, h);
}

#endif
