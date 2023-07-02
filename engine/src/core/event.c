#include <core/event.h>
#include <core/mem.h>

typedef struct registeredEvent {
	void * listener;
	event_signal_func callback;
} registered_event_t;

typedef struct eventCodeEntry {
	registered_event_t * events;
} event_code_entry_t;

#define MAX_MESSAGE_CODES (0xFFFF - 0xFF)

typedef struct eventSystemState {
	event_code_entry_t registered[MAX_MESSAGE_CODES];
} event_system_state_t;

static b8 initialized = FALSE;
static event_system_state_t state;
