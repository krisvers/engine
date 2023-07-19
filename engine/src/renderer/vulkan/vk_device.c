#include <defines.h>

#ifdef KPLATFORM_VULKAN

#include <renderer/vulkan/device.h>
#include <renderer/vulkan/vulkan_types.h>
#include <vulkan/vulkan.h>
#include <core/logger.h>
#include <core/string.h>
#include <core/mem.h>
#include <containers/dynarray.h>

typedef struct vulkanPhysicalDeviceRequirements {
	b8 graphics;
	b8 present;
	b8 compute;
	b8 transfer;
	// dynarray_t<const char *>
	dynarray_t * device_extension_names;
	b8 sampler_anisotrophy;
	b8 discrete;
} vulkan_physical_device_requirements_t;

typedef struct vulkanPhysicalDeviceQueueFamilyInfo {
	u32 graphics_index;
	u32 present_index;
	u32 compute_index;
	u32 transfer_index;
} vulkan_physical_device_queue_family_info_t;

static b8 vulkan_select_phys(vulkan_context_t * context);
static b8 vulkan_phys_meets_reqs(
	VkPhysicalDevice device, VkSurfaceKHR surface,
	const VkPhysicalDeviceProperties * props,
	const VkPhysicalDeviceFeatures * feats,
	const vulkan_physical_device_requirements_t * reqs,
	vulkan_physical_device_queue_family_info_t * out_queue_family_info,
	vulkan_swapchain_support_info_t * out_swapchain_support
);

b8 vulkan_device_create(vulkan_context_t * context) {
	if (!vulkan_select_phys(context)) {
		return FALSE;
	}

	return TRUE;
}

void vulkan_device_destroy(vulkan_context_t * context) {

}

void vulkan_device_query_swapchain_support(
    VkPhysicalDevice phys,
    VkSurfaceKHR surface,
    vulkan_swapchain_support_info_t * out_support_info
) {
	VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(phys, surface, &out_support_info->capabilities));
	VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(phys, surface, &out_support_info->format_count, NULL));
	if (out_support_info->format_count != 0) {
		if (out_support_info->formats == NULL) {
			out_support_info->formats = kmalloc(sizeof(VkSurfaceFormatKHR) * out_support_info->format_count, MEMORY_TAG_RENDERER);
		}
		VK_CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(
			phys, surface,
			&out_support_info->format_count,
			out_support_info->formats
		));
	}

	VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(
		phys, surface,
		&out_support_info->present_mode_count,
		NULL
	));

	if (out_support_info->present_mode_count != 0) {
		if (out_support_info->present_modes == NULL) {
			out_support_info->present_modes = kmalloc(sizeof(VkPresentModeKHR) * out_support_info->present_mode_count, MEMORY_TAG_RENDERER);
		}
		VK_CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(
			phys, surface,
			&out_support_info->present_mode_count,
			out_support_info->present_modes
		));
	}
}

static b8 vulkan_select_phys(vulkan_context_t * context) {
	u32 phys_count = 0;
	VK_CHECK(vkEnumeratePhysicalDevices(context->instance, &phys_count, NULL));
	if (phys_count == 0) {
		KFATAL("no Vulkan-supported physical devices found");
		return FALSE;
	}

	VkPhysicalDevice phys_devices[phys_count];
	VK_CHECK(vkEnumeratePhysicalDevices(context->instance, &phys_count, phys_devices));
	for (u32 i = 0; i < phys_count; ++i) {
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(phys_devices[i], &props);

		VkPhysicalDeviceFeatures feats;
		vkGetPhysicalDeviceFeatures(phys_devices[i], &feats);

		VkPhysicalDeviceMemoryProperties mem;
		vkGetPhysicalDeviceMemoryProperties(phys_devices[i], &mem);

		vulkan_physical_device_requirements_t reqs = {};
		reqs.graphics = TRUE;
		reqs.present = TRUE;
		reqs.sampler_anisotrophy = TRUE;
		reqs.discrete = TRUE;
		reqs.device_extension_names = dynarray_create(const char *);
		dynarray_push(reqs.device_extension_names, &VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		vulkan_physical_device_queue_family_info_t queue_info = {};
		
	}

	return TRUE;
}

static b8 vulkan_phys_meets_reqs(
	VkPhysicalDevice device, VkSurfaceKHR surface,
	const VkPhysicalDeviceProperties * props,
	const VkPhysicalDeviceFeatures * feats,
	const vulkan_physical_device_requirements_t * reqs,
	vulkan_physical_device_queue_family_info_t * out_queue_family_info,
	vulkan_swapchain_support_info_t * out_swapchain_support
) {
	out_queue_family_info->graphics_index = -1;
	out_queue_family_info->present_index = -1;
	out_queue_family_info->compute_index = -1;
	out_queue_family_info->transfer_index = -1;

	if (reqs->discrete) {
		if (props->deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
			KDEBUG("device %s is not a discrete GPU, skipping device", props->deviceName);
			return FALSE;
		}
	}

	u32 queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, NULL);
	VkQueueFamilyProperties queue_families[queue_family_count];
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families);
#if defined(DEBUG_FLAG)
	KDEBUG("device %s supports:", props->deviceName);
	KDEBUG("Graphics\t| Present\t| Compute\t| Transfer");
#endif
	u8 min_transfer_score = 255;
	for (u32 i = 0; i < queue_family_count; ++i) {
		u8 transfer_score = 0;

		if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			out_queue_family_info->graphics_index = 1;
			++transfer_score;
		}

		if (queue_families[i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
			out_queue_family_info->compute_index = 1;
			++transfer_score;
		}

		if (queue_families[i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
			if (transfer_score <= min_transfer_score) {
				min_transfer_score = transfer_score;
				out_queue_family_info->transfer_index = 1;
			}
		}

		VkBool32 supports_present = VK_FALSE;
		VK_CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, 1, surface, &supports_present));
		if (supports_present) {
			out_queue_family_info->present_index = i;
		}
	}

#if defined(DEBUG_FLAG)
	KDEBUG("       %u\t|       %u\t|       %u\t|        %u", (out_queue_family_info->graphics_index != -1), (out_queue_family_info->present_index != -1), (out_queue_family_info->compute_index != -1), (out_queue_family_info->transfer_index != -1));
#endif

	u8 supported = 0;
	if (!reqs->graphics || (reqs->graphics && out_queue_family_info->graphics_index != -1)) {
		supported |= 1;
	}
	if (!reqs->present || (reqs->present && out_queue_family_info->present_index != -1)) {
		supported |= 2;
	}
	if (!reqs->compute || (reqs->compute && out_queue_family_info->compute_index != -1)) {
		supported |= 4;
	}
	if (!reqs->transfer || (reqs->transfer && out_queue_family_info->transfer_index != -1)) {
		supported |= 8;
	}

	if (supported != 0xF) {
		KDEBUG("device %s does not meet queue requirements, skipping", props->deviceName);
		return FALSE;
	}

	vulkan_device_query_swapchain_support(device, surface, out_swapchain_support);

	if (out_swapchain_support->format_count < 1 || out_swapchain_support->present_mode_count < 1) {
		if (out_swapchain_support->formats != NULL) {
			kfree(out_swapchain_support->formats, sizeof(VkSurfaceFormatKHR) * out_swapchain_support->format_count, MEMORY_TAG_RENDERER);
		}
		if (out_swapchain_support->present_modes) {
			kfree(out_swapchain_support, sizeof(VkSurfaceFormatKHR) * out_swapchain_support->present_mode_count, MEMORY_TAG_RENDERER);
		}
		KDEBUG("required swapchain support not found, skipping");
		return FALSE;
	}

	if (reqs->device_extension_names != NULL) {
		u32 available_count = 0;
		VkExtensionProperties * available_extensions = NULL;
		VK_CHECK(vkEnumerateDeviceExtensionProperties(
			device, NULL,
			&available_count, NULL
		));
	}

	return TRUE;
}
#endif