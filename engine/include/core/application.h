#ifndef KENGINE_CORE_APPLICATION_H
#define KENGINE_CORE_APPLICATION_H

#include <defines.h>
#include <game_types.h>
#include <core/application_config.h>

b8 KAPI application_create(game_t * instance);
b8 KAPI application_run(void);

#endif
