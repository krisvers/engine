#include <core/application.h>
#include <core/logger.h>
#include <core/mem.h>
#include <core/event.h>
#include <core/input.h>
#include <platform/platform.h>

typedef struct applicationState {
	game_t * game_instance;
	b8 running;
	b8 suspended;
	platform_state_t platform;
	u16 w;
	u16 h;
	f64 last_time;
} application_state_t;

static b8 initialized = FALSE;
static application_state_t app_state;

b8 application_create(game_t * instance) {
	if (initialized) {
		KFATAL("[application_create(config)]:");
		KFATAL("already initialized");
		return FALSE;
	}

	app_state.game_instance = instance;

	// init systems
	log_init();
	input_init();

	// test
	KFATAL("test message %lf", 3.14);
	KERROR("test message %lf", 3.14);
	KWARN("test message %lf", 3.14);
	KINFO("test message %lf", 3.14);
	KDEBUG("test message %lf", 3.14);
	KTRACE("test message %lf", 3.14);

	app_state.running = TRUE;
	app_state.suspended = FALSE;

	if (!event_init()) {
		KFATAL("[application_create()]");
		KFATAL("event system failed to initialize");
		return FALSE;
	}

	if (!platform_startup(
		&app_state.platform,
		instance->app_config.name,
		instance->app_config.x, instance->app_config.y,
		instance->app_config.w, instance->app_config.h)
	) {
		KFATAL("[application_create()]");
		KFATAL("Failed to startup platform");
		return FALSE;
	}

	if (!app_state.game_instance->initialize(app_state.game_instance)) {
		KFATAL("[application_create()]");
		KFATAL("Game failed to initialize");
		return FALSE;
	}

	initialized = TRUE;
	return TRUE;
}

b8 application_run(void) {
	if (!initialized) {
		KFATAL("[application_run()]");
		KFATAL("Application was not initialized");
		return FALSE;
	}
	KLOG(memory_get_usage_cstr());

	while (app_state.running) {
		if (!platform_pump_messages(&app_state.platform)) {
			app_state.running = FALSE;
		}

		if (app_state.suspended) {
			continue;
		}

		f32 f = 1.0f / (app_state.game_instance->update_frequency) * 1000.0f;
		platform_sleep((u64) f);
		if (!app_state.game_instance->update(app_state.game_instance, 0.0f)) {
			KFATAL("[application_run()]");
			KFATAL("Game update failed!");
			app_state.running = FALSE;
			break;
		}

		if (!app_state.game_instance->render(app_state.game_instance, 0.0f)) {
			KFATAL("[application_run()]");
			KFATAL("Game render failed!");
			app_state.running = FALSE;
			break;
		}

		input_update(0);
	}

	app_state.running = FALSE;
	event_deinit();
	input_deinit();
	platform_shutdown(&app_state.platform);
	return TRUE;
}
