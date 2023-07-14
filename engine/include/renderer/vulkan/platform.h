#ifndef KENGINE_RENDERER_VULKAN_PLATFORM_H
#define KENGINE_RENDERER_VULKAN_PLATFORM_H

#include <defines.h>
#include <containers/dynarray.h>
#include <renderer/vulkan/vulkan_types.h>

b8 platform_create_vulkan_surface(platform_state_t * pstate, vulkan_context_t * context);
void platform_get_required_extension_names(dynarray_t ** array);

#endif