#include <renderer/vulkan/backend.h>
#include <renderer/renderer_types.h>
#include <platform/platform.h>
#include <renderer/vulkan/vulkan_types.h>
#include <vulkan/vulkan.h>
#include <core/logger.h>

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
    create_info.enabledExtensionCount = 0;
    create_info.ppEnabledExtensionNames = 0;
    create_info.enabledLayerCount = 0;
    create_info.ppEnabledLayerNames = 0;

    VkResult result = vkCreateInstance(&create_info, context.allocator, &context.instance);
    if (result != VK_SUCCESS) {
        KFATAL("[vulkan_renderer_backend_init(backend, application_name, pstate)]");
        KFATAL("VkCreateInstance failed with error %u", result);
        return FALSE;
    }

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