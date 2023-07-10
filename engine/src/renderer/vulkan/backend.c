#include <renderer/vulkan/backend.h>
#include <renderer/renderer_types.h>
#include <platform/platform.h>
#include <renderer/vulkan/platform.h>
#include <renderer/vulkan/vulkan_types.h>
#include <vulkan/vulkan.h>
#include <core/logger.h>
#include <core/string.h>
#include <containers/dynarray.h>

static vulkan_context_t context;

b8 vulkan_renderer_backend_init(renderer_backend_t * backend, const char * application_name, platform_state_t * pstate) {
	context.allocator = NULL;

	VkApplicationInfo app_info = { VK_STRUCTURE_TYPE_APPLICATION_INFO };
	app_info.apiVersion = VK_API_VERSION_1_2;
	app_info.pApplicationName = application_name;
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "kengine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

	VkInstanceCreateInfo create_info = { VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO };
	create_info.pApplicationInfo = &app_info;

	dynarray_t * required_extensions = dynarray_create(const char *);
	//dynarray_push(required_extensions, &VK_KHR_SURFACE_EXTENSION_NAME);
	platform_get_required_extension_names(&required_extensions);
#if defined(DEBUG_FLAG)
	dynarray_push(required_extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	KDEBUG("[vulkan_renderer_backend_init(backend, application_name, pstate)]");
	KDEBUG("%llu required extensions:", required_extensions->length);
	for (u64 i = 0; i < required_extensions->length; ++i) {
		KDEBUG("%s", ((const char **) required_extensions->array)[i]);
	}
#endif

	create_info.enabledExtensionCount = required_extensions->length;
	create_info.ppEnabledExtensionNames = required_extensions->array;

	dynarray_t * required_val_layer_names = NULL;
	u32 required_val_layer_count = 0;

#if defined(DEBUG_FLAG)
	KDEBUG("[vulkan_renderer_backend_init(backend, application_name, pstate)]");
	KDEBUG("vulkan validation layers enabled");

	required_val_layer_names = dynarray_create(const char *);
	dynarray_push(required_val_layer_names, &"VK_LAYER_KHRONOS_validation");
	required_val_layer_count = required_val_layer_names->length;

	u32 available_count = 0;
	VK_CHECK(vkEnumerateInstanceLayerProperties(&available_count, NULL));
	dynarray_t * available_layers = dynarray_reserve(VkLayerProperties, available_count);
	VK_CHECK(vkEnumerateInstanceLayerProperties(&available_count, available_layers->array));

	for (u32 i = 0; i < required_val_layer_count; ++i) {
		KDEBUG("searching for layer: %s", ((const char **) required_val_layer_names->array)[i]);
		b8 found = FALSE;
		for (u32 j = 0; j < available_count; ++j) {
			if (kstrcmp(((const char **) required_val_layer_names->array)[i], ((VkLayerProperties *) available_layers->array)[j].layerName) == 0) {
				found = TRUE;
				KDEBUG("found %s", ((const char **) required_val_layer_names->array)[i]);
				break;
			}
		}

		if (!found) {
			KFATAL("[vulkan_renderer_backend_init(backend, application_name, pstate)]");
			KFATAL("failed to find validation layer %s", required_val_layer_names[i]);
			return FALSE;
		}
	}
	KDEBUG("found all required validation layers");
#endif

	create_info.enabledLayerCount = required_val_layer_count;
	create_info.ppEnabledLayerNames = (required_val_layer_names == NULL) ? NULL : required_val_layer_names->array;

	VK_CHECK_MSG(vkCreateInstance(&create_info, context.allocator, &context.instance), "failed to create vulkan instance");

	return TRUE;
}

void vulkan_renderer_backend_deinit(renderer_backend_t * backend) {
	
}

void vulkan_renderer_backend_on_resize(renderer_backend_t * backend, u32 w, u32 h) {

}

b8 vulkan_renderer_backend_frame_begin(renderer_backend_t * backend, f64 delta_time) {
	return TRUE;
}

b8 vulkan_renderer_backend_frame_end(renderer_backend_t * backend, f64 delta_time) {
	return TRUE;
}