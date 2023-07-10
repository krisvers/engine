#ifndef KENGINE_CORE_EVENT_H
#define KENGINE_CORE_EVENT_H

#include <defines.h>

typedef struct eventContext {
	union {
		i64 i64[2];
		i32 i32[4];
		i16 i16[8];
		i8 i8[16];
	
		u64 u64[2];
		u32 u32[4];
		u16 u16[8];
		u8 u8[16];

		f64 f64[2];
		f32 f32[4];
	
		char c[16];

		void * vptr[2];
	} data;
} event_context_t;

typedef b8 (*event_signal_func)(u16 code, void * sender, void * listener, event_context_t ctx);

b8 event_init(void);
void event_deinit(void);

b8 KAPI event_register(u16 code, void * listener, event_signal_func callback);
b8 KAPI event_unregister(u16 code, void * listener, event_signal_func callback);
b8 KAPI event_fire(u16 code, void * sender, event_context_t context);

typedef enum systemEventCodeEnum {
	EVENT_CODE_APPLICATION_QUIT = 0x01,
	EVENT_CODE_KEY_PRESSED = 0x02,
	EVENT_CODE_KEY_RELEASED = 0x03,
	EVENT_CODE_MOUSE_BUTTON_PRESSED = 0x04,
	EVENT_CODE_MOUSE_BUTTON_RELEASED = 0x05,
	EVENT_CODE_MOUSE_MOVED = 0x06,
	EVENT_CODE_MOUSE_SCROLL = 0x07,
	
	EVENT_CODE_MAX = 0xFF
} event_system_code_enum;

#endif
