#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>
#include <vulkan_context.h>

#ifndef NDEBUG

#include <stdio.h>
#include <term.h>

static const char *validationLayers[] = {
        "VK_LAYER_KHRONOS_validation",
};

static bool check_validation_support() {
    VkResult           res;
    uint32_t           layerCount;
    VkLayerProperties *layers;

    res = vkEnumerateInstanceLayerProperties(&layerCount, NULL);
    if (res != VK_SUCCESS || layerCount == 0) { return false; }

    layers = malloc(layerCount * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&layerCount, layers);
    if (res != VK_SUCCESS) {
        free(layers);
        return false;
    }

    for (size_t i = 0; i < ARRAY_SIZE(validationLayers); ++i) {
        bool layerFound = false;
        for (size_t j = 0; j < layerCount; ++j) {
            if (strcmp(validationLayers[i], layers[j].layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            free(layers);
            return false;
        }
    }

    free(layers);
    return true;
}

static bool check_debug_utils_support() {
    VkResult               res;
    uint32_t               extCount = 0;
    VkExtensionProperties *exts     = NULL;

    res = vkEnumerateInstanceExtensionProperties(NULL, &extCount, NULL);
    if (res != VK_SUCCESS || extCount == 0) { return false; }

    exts = malloc(sizeof(VkExtensionProperties) * extCount);
    vkEnumerateInstanceExtensionProperties(NULL, &extCount, exts);
    if (res != VK_SUCCESS) {
        free(exts);
        return false;
    }

    bool found = false;
    for (size_t i = 0; i < extCount; ++i) {
        if (strcmp(exts[i].extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0) {
            found = true;
            goto exit;
        }
    }

exit:
    free(exts);
    return found;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL
debug_callback(VkDebugUtilsMessageSeverityFlagsEXT         messageSeverity,
               VkDebugUtilsMessageTypeFlagsEXT             messageType,
               const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
               void                                       *pUserData) {
    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            fprintf(stderr, ANSI_BLUE "[VERBOSE] %s\n" ANSI_RESET, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            fprintf(stderr, ANSI_GREEN "[INFO] %s\n" ANSI_RESET, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            fprintf(stderr, ANSI_YELLOW "[WARNING] %s\n" ANSI_RESET, pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            fprintf(stderr, ANSI_RED "[ERROR] %s\n" ANSI_RESET, pCallbackData->pMessage);
            break;
        default:
            printf(stderr, ANSI_RED "[UNKNOWN] %s\n" ANSI_RESET, pCallbackData->pMessage);
            break;
    }
    return VK_FALSE;
}

VkResult create_debug_messenger_ext(VkInstance                                instance,
                                    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                    const VkAllocationCallbacks              *pAllocator,
                                    VkDebugUtilsMessengerEXT                 *pDebugMessenger) {
    PFN_vkCreateDebugUtilsMessengerEXT f = (PFN_vkCreateDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (f == NULL) { return VK_ERROR_EXTENSION_NOT_PRESENT; }
    return f(instance, pCreateInfo, pAllocator, pDebugMessenger);
}

void destroy_debug_messenger_ext(VkInstance                   instance,
                                 VkDebugUtilsMessengerEXT     debugMessenger,
                                 const VkAllocationCallbacks *pAllocator) {
    PFN_vkDestroyDebugUtilsMessengerEXT f = (PFN_vkDestroyDebugUtilsMessengerEXT)
            vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (f == NULL) { return; }
    f(instance, debugMessenger, pAllocator);
}

static int create_dbg_messenger(RendererState *r) {
    VkResult                           res;
    VkDebugUtilsMessengerCreateInfoEXT msgCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};

    msgCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    msgCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    msgCreateInfo.pfnUserCallback = debug_callback;
    msgCreateInfo.pUserData       = NULL;

    res = create_debug_messenger_ext(r->instance, &msgCreateInfo, NULL, &r->dbgMessenger);
    if (res != VK_SUCCESS) { return -1; }

    return 0;
}

#endif

#define EXT_EXTRA 4

static int create_instance(RendererState *r) {
    VkResult             res;
    uint32_t             glfwExtCount       = 0;
    const char         **glfwExts           = NULL;
    uint32_t             extCount           = 0;
    const char         **exts               = NULL;
    VkInstanceCreateInfo instanceCreateInfo = {.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    VkApplicationInfo    appInfo            = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO};

    glfwExts = glfwGetRequiredInstanceExtensions(&glfwExtCount);
    extCount = glfwExtCount;
    exts     = malloc(sizeof(char *) * (extCount + EXT_EXTRA));
    for (uint32_t i = 0; i < glfwExtCount; ++i) { exts[i] = glfwExts[i]; }

    appInfo.pApplicationName   = "macrochip";
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pEngineName        = "macrochip-engine";
    appInfo.engineVersion      = VK_MAKE_VERSION(0, 0, 1);
    appInfo.apiVersion         = VK_API_VERSION_1_0;

#ifndef NDEBUG
    if (check_validation_support()) {
        if (check_debug_utils_support()) { exts[extCount++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME; }
        instanceCreateInfo.enabledLayerCount   = ARRAY_SIZE(validationLayers);
        instanceCreateInfo.ppEnabledLayerNames = validationLayers;

        VkDebugUtilsMessengerCreateInfoEXT msgCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};

        msgCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        msgCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        msgCreateInfo.pfnUserCallback = debug_callback;
        msgCreateInfo.pUserData       = NULL;

        instanceCreateInfo.pNext = &msgCreateInfo;

    } else {
        instanceCreateInfo.enabledLayerCount   = 0;
        instanceCreateInfo.ppEnabledLayerNames = NULL;
    }
#else
    instanceCreateInfo.enabledLayerCount   = 0;
    instanceCreateInfo.ppEnabledLayerNames = NULL;
#endif

    instanceCreateInfo.pApplicationInfo        = &appInfo;
    instanceCreateInfo.enabledExtensionCount   = extCount;
    instanceCreateInfo.ppEnabledExtensionNames = exts;

    res = vkCreateInstance(&instanceCreateInfo, NULL, &r->instance);
    if (res != VK_SUCCESS) {
        free(exts);
        return -1;
    }
    free(exts);

#ifndef NDEBUG
    if (create_dbg_messenger(r) < 0) { return -1; }
#endif

    return 0;
}

int renderer_init(RendererState *r, Window *win) {
    int res = 0;

    r->window = win;
    res       = create_instance(r);
    if (res < 0) { goto exit_err; }

    return 0;
exit_err:
    renderer_terminate(r);
    return res;
}

void renderer_terminate(RendererState *r) {
#ifndef NDEBUG
    if (r->dbgMessenger != VK_NULL_HANDLE) {
        destroy_debug_messenger_ext(r->instance, r->dbgMessenger, NULL);
    }
#endif
    if (r->instance != VK_NULL_HANDLE) { vkDestroyInstance(r->instance, NULL); }
}