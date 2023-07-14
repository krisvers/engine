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

typedef struct vulkanSwapchainSupportInfo {
	VkSurfaceCapabilitiesKHR capabilities;
	u32 format_count;
	VkSurfaceFormatKHR * formats;
	u32 present_mode_count;
	VkPresentModeKHR * present_modes;
} vulkan_swapchain_support_info_t;

typedef struct vulkanDevice {
	VkPhysicalDevice phys;
	VkDevice logi;
} vulkan_device_t;

typedef struct vulkanContext {
	VkInstance instance;
	VkAllocationCallbacks * allocator;
#if defined(DEBUG_FLAG)
	VkDebugUtilsMessengerEXT debug_messenger;
#endif
	vulkan_device_t device;
	VkSurfaceKHR surface;
} vulkan_context_t;

#endif