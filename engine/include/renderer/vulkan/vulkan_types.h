#ifndef KENGINE_RENDERER_VULKAN_VULKAN_TYPES_H
#define KENGINE_RENDERER_VULKAN_VULKAN_TYPES_H

#include <defines.h>
#include <vulkan/vulkan.h>
#include <core/assert.h>

#define VK_CHECK(result)                \
	{                                   \
		KASSERT(result == VK_SUCCESS);  \
	}

#define VK_CHECK_MSG(result, msg)               \
	{                                           \
		KASSERT_MSG(result == VK_SUCCESS, msg); \
	}

typedef struct vulkanContext {
	VkInstance instance;
	VkAllocationCallbacks * allocator;
#if defined(DEBUG_FLAG)
	VkDebugUtilsMessengerEXT debug_messenger;
#endif
} vulkan_context_t;

#endif