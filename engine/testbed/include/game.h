#ifndef KTESTBED_GAME_H
#define KTESTBED_GAME_H

#include <defines.h>
#include <game_types.h>

typedef struct gameState {
	f32 delta_time;
} game_state_t; 

b8 game_initialize(game_t * instance);
b8 game_update(game_t * instance, f64 delta_time);
b8 game_render(game_t * instance, f64 delta_time);

#endif
