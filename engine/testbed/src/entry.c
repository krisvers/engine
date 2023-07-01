#include <core/application.h>
#include <core/logger.h>
#include <platform/platform.h>
#include <defines.h>
#include <stdio.h>
#include <game_types.h>
#include <entry.h>
#include <game.h>

enum {
	FAILURE_APPLICATION_CREATION = -3,
	FAILURE_GAME_FUNC_POINTERS = -2,
	FAILURE_GAME_CREATION = -1,
	FAILURE_APPLICATION_RUN = 1,
};

b8 game_create(game_t * out_game) {
	out_game->app_config.x = 100;
	out_game->app_config.y = 100;
	out_game->app_config.w = 800;
	out_game->app_config.h = 600;
	out_game->app_config.name = "app";
	out_game->update = game_update;
	out_game->render = game_render;
	out_game->initialize = game_initialize;
	out_game->state = platform_malloc(sizeof(game_state_t), FALSE);

	return TRUE;
}

int main(void) {
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

	// Game loop
	printf("Running application\n");
	if (!application_run()) {
		fprintf(stderr, "Application exited unsuccessfuly\n");
		return FAILURE_APPLICATION_RUN;
	}
	printf("Application was successful\n");

	return 0;
}
