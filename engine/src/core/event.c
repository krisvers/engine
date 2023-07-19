#include <core/event.h>
#include <core/logger.h>
#include <core/mem.h>
#include <containers/dynarray.h>

typedef struct registeredEvent {
	void * listener;
	event_signal_func callback;
} registered_event_t;

typedef struct eventCodeEntry {
	dynarray_t/*<registered_event_t>*/ * events;
} event_code_entry_t;

#define MAX_MESSAGE_CODES (0xFFFF)

typedef struct eventSystemState {
	event_code_entry_t registered[MAX_MESSAGE_CODES];
} event_system_state_t;

static b8 initialized = FALSE;
static event_system_state_t state;

b8 event_init(void) {
	if (initialized) {
		return FALSE;
	}
	kmemzero(&state, sizeof(state));
	initialized = TRUE;

	return TRUE;
}

void event_deinit(void) {
	for (u16 i = 0; i < MAX_MESSAGE_CODES; ++i) {
		if (state.registered[i].events != NULL) {
			dynarray_destroy(state.registered[i].events);
			state.registered[i].events = (dynarray_t *) NULL;
		}
	}
}

b8 event_register(u16 code, void * listener, event_signal_func callback) {
	if (!initialized) {
		return FALSE;
	}

	if (state.registered[code].events == NULL) {
		state.registered[code].events = dynarray_create(registered_event_t);
	}

	u64 registered_count = state.registered[code].events->length;
	registered_event_t * events = (registered_event_t *) state.registered[code].events->array;
	for (u64 i = 0; i < registered_count; ++i) {
		if (events[i].listener == listener) {
			KWARN("[event_register(code, listener, callback)]");
			KWARN("event %u already registered with this listener", code);
			return FALSE;
		}
	}

	registered_event_t event;
	event.listener = listener;
	event.callback = callback;
	dynarray_push(state.registered[code].events, event, registered_event_t);
	
	return TRUE;
}

b8 event_unregister(u16 code, void * listener, event_signal_func callback) {
	if (!initialized) {
		return FALSE;
	}

	if (state.registered[code].events == NULL) {
		KWARN("[event_unregister(code, listener, callback)]");
		KWARN("event %u has no listeners");
		return FALSE;
	}

	u64 registered_count = state.registered[code].events->length;
	registered_event_t * events = (registered_event_t *) state.registered[code].events->array;
	for (u64 i = 0; i < registered_count; ++i) {
		registered_event_t e = events[i];
		if (e.listener == listener && e.callback == callback) {
			dynarray_remove_at(state.registered[code].events, i, registered_event_t);
			return TRUE;
		}
	}

	KWARN("[event_unregister(code, listener, callback)]");
	KWARN("event %u does not have listener %p and callback %p", code, listener, callback);
	return FALSE;
}

b8 event_fire(u16 code, void * sender, event_context_t context) {
	if (!initialized) {
		return FALSE;
	}

	if (state.registered[code].events == NULL) {
		//KWARN("[event_fire(code, sender, context)]");
		//KWARN("event %u has nothing registered for it");
		return FALSE;
	}

	u64 registered_count = state.registered[code].events->length;
	registered_event_t * events = (registered_event_t *) state.registered[code].events->array;
	for (u64 i = 0; i < registered_count; ++i) {
		registered_event_t e = events[i];
		if (e.callback(code, sender, e.listener, context)) {
			return TRUE;
		}
	}

	return FALSE;
}
