#ifndef KENGINE_RENDERER_VULKAN_DEVICE_H
#define KENGINE_RENDERER_VULKAN_DEVICE_H

#include <defines.h>
#include <renderer/vulkan/vulkan_types.h>
#include <vulkan/vulkan.h>

b8 vulkan_device_create(vulkan_context_t * context);
void vulkan_device_destroy(vulkan_context_t * context);
void vulkan_device_query_swapchain_support(
    VkPhysicalDevice phys,
    VkSurfaceKHR surface,
    vulkan_swapchain_support_info_t * out_support_info
);

#endif