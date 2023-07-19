#include <core/application.h>
#include <core/logger.h>
#include <core/mem.h>
#include <core/event.h>
#include <core/input.h>
#include <core/clock.h>
#include <platform/platform.h>
#include <renderer/frontend.h>
#include <containers/camera.h>

typedef struct applicationState {
	game_t * game_instance;
	b8 running;
	b8 suspended;
	platform_state_t platform;
	u16 w;
	u16 h;
	clock_t clock;
	f64 last_time;
	camera_t * camera;
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
	app_state.camera = instance->camera;
	instance->running = TRUE;

	// init systems
	log_init();
	#ifdef DEBUG_FLAG
	log_set_logfile("log.txt", LOG_LEVEL_INFO);
	#endif
	input_init();

	// test
	KINFO("test message %lf", 3.14);

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

	renderer_set_camera(app_state.camera);

	initialized = TRUE;
	return TRUE;
}

static f64 last_print = 0;

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
	KLOG("%s", memory_get_usage_cstr());

	while (app_state.running) {
		if (!platform_pump_messages(&app_state.platform)) {
			app_state.running = FALSE;
		}

		if (app_state.suspended) {
			continue;
		}

		clock_update(&app_state.clock);
		f64 frame_start = platform_get_absolute_time();
		f64 current_time = app_state.clock.elapsed;
		f64 delta = (current_time - app_state.last_time);

		if (!app_state.game_instance->update(app_state.game_instance, delta)) {
			KFATAL("[application_run()]");
			KFATAL("Game update failed!");
			app_state.running = FALSE;
			break;
		}

		if (!app_state.game_instance->running) {
			KDEBUG("[application_run()]");
			KDEBUG("closing game");
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
		packet.mesh = mesh_create();
		vertex_t a = {
			{ -1, -1, 0 },
			{ 1, 1, 0 },
			{ 0, 0 },
			1,
		};
		vertex_t b = {
			{ -1, 1, 0 },
			{ 0, 1, 1 },
			{ 0, 1 },
			1,
		};
		vertex_t c = {
			{ 1, -1, 0 },
			{ 1, 0, 1 },
			{ 1, 0 },
			1,
		};
		vertex_t d = {
			{ 1, 1, 0 },
			{ 1, 1, 1 },
			{ 1, 1 },
			1,
		};
		mesh_push_vertex(packet.mesh, &a);
		mesh_push_vertex(packet.mesh, &b);
		mesh_push_vertex(packet.mesh, &c);
		mesh_push_vertex(packet.mesh, &d);
		indice_t indice = { 0, 1, 3 };
		mesh_push_indices_value(packet.mesh, indice);
		indice.x = 0;
		indice.y = 3;
		indice.z = 2;
		mesh_push_indices_value(packet.mesh, indice);
		if (!renderer_draw_frame(&packet)) {
		}
		mesh_destroy(packet.mesh);

		platform_swap_buffers(&app_state.platform);

		f64 frame_end = platform_get_absolute_time();
		f32 frame_elapsed = frame_end - frame_start;
		if (frame_end - last_print > 0.5) {
			f32 fps = 1 / frame_elapsed;
			KDEBUG("FPS: %f", fps);
			last_print = frame_end;
		}
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

	log_unset_logfile();
	app_state.running = FALSE;
	event_deinit();
	input_deinit();
	renderer_deinit();
	platform_shutdown(&app_state.platform);
	return TRUE;
}
