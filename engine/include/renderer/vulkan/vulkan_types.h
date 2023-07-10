#ifndef KENGINE_RENDERER_VULKAN_VULKAN_TYPES_H
#define KENGINE_RENDERER_VULKAN_VULKAN_TYPES_H

#include <defines.h>
#include <vulkan/vulkan.h>

typedef struct vulkanContext {
    VkInstance instance;
    VkAllocationCallbacks * allocator;
} vulkan_context_t;

#endif