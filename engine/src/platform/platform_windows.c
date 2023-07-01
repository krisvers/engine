#include <platform/platform.h>

#if KPLATFORM_WINDOWS

#include <windows.h>
#include <windowsx.h>
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

internal_state_t * current_state;
static f64 clock_frequency;
static LARGE_INTEGER start_time;

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

	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	clock_frequency = 1.0f / (f64) frequency.QuadPart;
	QueryPerformanceCounter(&start_time);

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
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
	static u8 levels[6] = { 0x40, 0x04, 0x06, 0x02, 0x01, 0x08 };
	if (color < LOG_LEVEL_TRACE) {
		SetConsoleTextAttribute(h_console, levels[color]);
	}

#if DEBUG_FLAG
	OutputDebugStringA(message);
#endif

	DWORD length = strlen(message);
	LPDWORD written = 0;
	WriteConsoleA(h_console, message, length, written, 0);
}

void platform_console_write_error(const char * message, u8 color) {
	HANDLE h_console = GetStdHandle(STD_ERROR_HANDLE);
	static u8 levels[6] = { 0x40, 0x04, 0x06, 0x02, 0x01, 0x08 };
	if (color < LOG_LEVEL_TRACE) {
		SetConsoleTextAttribute(h_console, levels[color]);
	}

#if DEBUG_FLAG
	OutputDebugStringA(message);
#endif

	DWORD length = strlen(message);
	LPDWORD written = 0;
	WriteConsoleA(h_console, message, length, written, 0);
}

f64 platform_get_absolute_time(void) {
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	return (f64) now.QuadPart * clock_frequency;
}

void platform_sleep(u64 ms) {
	Sleep(ms);
}

static void glfw_error_handler(int error, const char * desc) {
	KERROR("GLFW error: %i, \"%s\"", error, desc);
}

static void glfw_key_handler(GLFWwindow * window, int key, int scancode, int action, int mods) {
	KLOG("window %p: key %d, %d, %d, %d pressed", (void *) window, key, scancode, action, mods);
}

static void glfw_mouse_pos_handler(GLFWwindow * window, f64 x, f64 y) {
	KLOG("window %p: mouse moved (%lf, %lf)", (void *) window, x, y);
}

static void glfw_mouse_button_handler(GLFWwindow * window, int button, int action, int mods) {
	KLOG("window %p: mouse button %d, %d, %d pressed", (void *) window, button, action, mods);
}

static void glfw_mouse_scroll_handler(GLFWwindow * window, double dx, double dy) {
	KLOG("window %p: mouse scroll %lf %lf", (void *) window, dx, dy);
}

static void glfw_window_close_handler(GLFWwindow * window) {
	KLOG("window %p is being closed", (void *) window);
	glfwDestroyWindow(window);
	current_state->glfw_win = NULL;
}

#endif
