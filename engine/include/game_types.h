#ifndef KENGINE_GAME_TYPE_H
#define KENGINE_GAME_TYPE_H

#include <core/application_config.h>
#include <containers/camera.h>

typedef struct game {
	application_config_t app_config;
	b8 (* initialize)(struct game * instance);
	b8 (* update)(struct game * instance, f64 delta_time);
	b8 (* render)(struct game * instance, f64 delta_time);
	void * state;
	u16 update_frequency;
	camera_t * camera;
} game_t;

typedef b8 (* game_initialize_func)(game_t * instance);
typedef b8 (* game_update_func)(game_t * instance, f64 delta_time);
typedef b8 (* game_render_func)(game_t * instance, f64 delta_time);

#endif
