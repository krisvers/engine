#ifndef KENGINE_PLATFORM_PLATFORM_H
#define KENGINE_PLATFORM_PLATFORM_H

#include <defines.h>

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

void KAPI * platform_malloc(u64 size, b8 aligned);
void KAPI platform_free(void * ptr, b8 aligned);
void * platform_memset(void * dst, i32 value, u64 size);
void * platform_memcpy(void * dst, const void * src, u64 size);
void * platform_memzero(void * dst, u64 size);

void platform_console_write(const char * message, u8 color);
void platform_console_write_error(const char * message, u8 color);

f64 platform_get_absolute_time(void);
void platform_sleep(u64 ms);

#endif
