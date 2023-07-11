#include <core/application.h>
#include <core/logger.h>
#include <core/mem.h>
#include <core/event.h>
#include <core/input.h>
#include <core/clock.h>
#include <platform/platform.h>
#include <renderer/frontend.h>

typedef struct applicationState {
	game_t * game_instance;
	b8 running;
	b8 suspended;
	platform_state_t platform;
	u16 w;
	u16 h;
	clock_t clock;
	f64 last_time;
} application_state_t;

static b8 initialized = FALSE;
static application_state_t app_state;

b8 application_create(game_t * instance) {
	if (initialized) {
		KFATAL("[application_create(instance)]:");
		KFATAL("already initialized");
		return FALSE;
	}

	app_state.game_instance = instance;

	// init systems
	log_init();
	log_set_logfile("log.txt");
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
		KFATAL("[application_create(instance)]");
		KFATAL("event system failed to initialize");
		return FALSE;
	}

	if (!platform_startup(
		&app_state.platform,
		instance->app_config.name,
		instance->app_config.x, instance->app_config.y,
		instance->app_config.w, instance->app_config.h)
	) {
		KFATAL("[application_create(instance)]");
		KFATAL("Failed to startup platform");
		return FALSE;
	}

	if (!renderer_init(app_state.game_instance->app_config.name, &app_state.platform)) {
		KFATAL("[application_create(instance)]");
		KFATAL("renderer failed to initialize");
		return FALSE;
	}

	if (!app_state.game_instance->initialize(app_state.game_instance)) {
		KFATAL("[application_create(instance)]");
		KFATAL("Game failed to initialize");
		return FALSE;
	}

	initialized = TRUE;
	return TRUE;
}

b8 application_run(void) {
	clock_start(&app_state.clock);
	clock_update(&app_state.clock);
	app_state.last_time = app_state.clock.elapsed;

	f64 running_time = 0;
	u8 frame_count = 0;
	f64 target_spf = 1.0f / 60.0f;

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

		clock_update(&app_state.clock);
		f64 current_time = app_state.clock.elapsed;
		f64 delta = (current_time - app_state.last_time);
		f64 frame_start = platform_get_absolute_time();

		/*f32 f = 1.0f / (app_state.game_instance->update_frequency) * 1000.0f;
		platform_sleep((u64) f);*/
		if (!app_state.game_instance->update(app_state.game_instance, delta)) {
			KFATAL("[application_run()]");
			KFATAL("Game update failed!");
			app_state.running = FALSE;
			break;
		}

		if (!app_state.game_instance->render(app_state.game_instance, delta)) {
			KFATAL("[application_run()]");
			KFATAL("Game render failed!");
			app_state.running = FALSE;
			break;
		}

		// refactor this!!
		render_packet_t packet;
		packet.delta_time = delta;
		renderer_draw_frame(&packet);

		f64 frame_end = platform_get_absolute_time();
		f64 frame_elapsed = frame_end - frame_start;
		running_time += frame_elapsed;
		f64 remaining_seconds = target_spf - frame_elapsed;

		if (remaining_seconds > 0) {
			u64 remaining_ms = remaining_seconds * 1000;

			b8 frame_cap = FALSE;
			if (remaining_ms > 0 && frame_cap) {
				platform_sleep(remaining_ms);
			}

			++frame_count;
		}

		input_update(delta);

		app_state.last_time = current_time;
	}

	app_state.running = FALSE;
	event_deinit();
	input_deinit();
	renderer_deinit();
	platform_shutdown(&app_state.platform);
	return TRUE;
}
