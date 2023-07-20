#ifndef KENGINE_PLATFORM_PLATFORM_H
#define KENGINE_PLATFORM_PLATFORM_H

#include <defines.h>
#include <stdio.h>

typedef struct platformState {
	void * internal_state;
} platform_state_t;

b8 platform_startup(
	platform_state_t * platform_state,
	const char * app_name,
	i32 x, i32 y, i32 w, i32 h
);

void platform_shutdown(platform_state_t * platform_state);
b8 platform_pump_messages(platform_state_t * platform_state);
void platform_swap_buffers(platform_state_t * platform_state);

void platform_set_cursor(u8 value);
u8 platform_get_cursor(void);

void * platform_malloc(u64 size, b8 aligned);
void platform_free(void * ptr, b8 aligned);
void * platform_memset(void * dst, i32 value, u64 size);
void * platform_memcpy(void * dst, const void * src, u64 size);
void * platform_memzero(void * dst, u64 size);

void platform_console_write_length(u8 * message, u64 length, u8 color);
void platform_console_write(const char * message, u8 color);
void platform_console_write_error(const char * message, u8 color);

f64 platform_get_absolute_time(void);
void platform_sleep(u64 ms);

#if defined(KPLATFORM_WINDOWS)
	#include <windows.h>
	typedef HANDLE file_desc_t;
#elif defined(KPLATFORM_LINUX)
	typedef struct {
		int fildes;
	} file_desc_t;
#else
#error "Unsupported platform file API"
#endif

/*
	return values that are non-zero are errors
*/
int platform_file_open(file_desc_t * outfd, char * filename, u8 op);
int platform_file_close(file_desc_t fd);
int platform_file_read(file_desc_t fd, u64 length, u8 * buffer);
int platform_file_write(file_desc_t fd, u64 length, u8 * buffer);
int platform_file_length(u64 * outlen, file_desc_t fd);
int platform_file_last_modification(f64 * outmod, file_desc_t fd, char * path);

#endif
