#ifndef VULKAN_INSTANCE_H
#define VULKAN_INSTANCE_H

#include <vulkan/vulkan.h>
#include <window.h>

typedef struct {
    Window    *window;
    VkInstance instance;

#ifndef NDEBUG
    VkDebugUtilsMessengerEXT dbgMessenger;
#endif
} RendererState;

int  renderer_init(RendererState *r, Window *win);
void renderer_terminate(RendererState *r);

#endif