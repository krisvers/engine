#include <core/application.h>
#include <core/logger.h>
#include <core/mem.h>
#include <platform/platform.h>
#include <defines.h>
#include <stdio.h>
#include <game_types.h>
#include <entry.h>
#include <game.h>
#include <config.h>

enum {
	FAILURE_APPLICATION_CREATION = -3,
	FAILURE_GAME_FUNC_POINTERS = -2,
	FAILURE_GAME_CREATION = -1,
	FAILURE_APPLICATION_RUN = 1,
};

b8 game_create(game_t * out_game) {
	out_game->app_config.x = CONFIG_WINDOW_X;
	out_game->app_config.y = CONFIG_WINDOW_Y;
	out_game->app_config.w = CONFIG_WINDOW_WIDTH;
	out_game->app_config.h = CONFIG_WINDOW_HEIGHT;
	out_game->app_config.name = CONFIG_WINDOW_NAME;
	out_game->initialize = game_initialize;
	out_game->update = game_update;
	out_game->render = game_render;
	out_game->state = kmalloc(sizeof(game_state_t), MEMORY_TAG_GAME);

	return TRUE;
}

int main(void) {
	memory_init();

	printf("%zu\n", sizeof(u64));

	game_t instance;
	if (!game_create(&instance)) {
		fprintf(stderr, "Game failed to create\n");
		return FAILURE_GAME_CREATION;
	}

	if (instance.update == NULL || instance.render == NULL || instance.initialize == NULL) {
		fprintf(stderr, "Game failed to assign function pointers\n");
		return FAILURE_GAME_FUNC_POINTERS;
	}

	// Initialization
	printf("Creating application\n");
	if (!application_create(&instance)) {
		fprintf(stderr, "Application failed to create\n");
		return FAILURE_APPLICATION_CREATION;
	}

	void * ptr = kmalloc((u64) 0x2FFFFFFFF, MEMORY_TAG_UNKNOWN);

	// Game loop
	printf("Running application\n");
	if (!application_run()) {
		fprintf(stderr, "Application exited unsuccessfuly\n");
		return FAILURE_APPLICATION_RUN;
	}
	printf("Application was successful\n");

	kfree(ptr, (u64) 0x2FFFFFFFF, MEMORY_TAG_DYNAMIC_ARRAY);
	memory_deinit();

	return 0;
}
