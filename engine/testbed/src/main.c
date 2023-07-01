#include <stdio.h>
#include <defines.h>
#include <core/logger.h>
#include <platform/platform.h>
#include <config.h>
#include <game_types.h>
#include <entry.h>
#include <game.h>

b8 create_game(game_t * out_game) {
	out_game->app_config.x = CONFIG_WINDOW_X;
	out_game->app_config.y = CONFIG_WINDOW_Y;
	out_game->app_config.w = CONFIG_WINDOW_WIDTH;
	out_game->app_config.h = CONFIG_WINDOW_HEIGHT;
	out_game->app_config.name = CONFIG_WINDOW_NAME;
	out_game->initialize = game_initialize;
	out_game->update = game_update;
	out_game->render = game_render;
	out_game->state = platform_malloc(sizeof(game_state_t), FALSE);

	return TRUE;
}
