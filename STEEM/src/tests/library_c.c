#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const char* validationLayers[] = {
    "VK_LAYER_KHRONOS_validation"
};

const char* deviceExtensions[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != NULL) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != NULL) {
        func(instance, debugMessenger, pAllocator);
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        fprintf(stderr, "validation layer: %s\n", pCallbackData->pMessage);
    }
    return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo)
{
    createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo->flags = 0;
    createInfo->pNext = NULL;
    createInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo->pfnUserCallback = debugCallback;
}

int clamp(int d, int min, int max)
{
    const int t = d < min ? min : d;
    return t > max ? max : t;
}

int main()
{
    VkDebugUtilsMessengerEXT debugMessenger;
    VkInstance Instance;
    VkSurfaceKHR surface;
    VkSwapchainKHR swapChain;
    VkImage* swapChainImages = NULL;
    VkImageView* swapChainImageViews = NULL;
    VkDevice device;
    GLFWwindow* window = NULL;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    VkQueue presentQueue;
    VkQueue graphicsQueue;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    const int HEIGHT = 640;
    const int WIDTH = 480;
    window = glfwCreateWindow(HEIGHT, WIDTH, "Window Title", NULL, NULL);

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo InstanceCreateInfo = {};
    InstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    InstanceCreateInfo.pApplicationInfo = &appInfo;

    uint32_t count;
    const char** extensions = glfwGetRequiredInstanceExtensions(&count);

    InstanceCreateInfo.enabledExtensionCount = count;
    InstanceCreateInfo.ppEnabledExtensionNames = extensions;
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    InstanceCreateInfo.enabledLayerCount = 1;
    InstanceCreateInfo.ppEnabledLayerNames = validationLayers;

    populateDebugMessengerCreateInfo(&debugCreateInfo);
    InstanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(&createInfo);
    vkCreateInstance(&InstanceCreateInfo, NULL, &Instance);
    CreateDebugUtilsMessengerEXT(Instance, &createInfo, NULL, &debugMessenger);

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(Instance, &deviceCount, NULL);

    VkPhysicalDevice* PhysicalDevices = malloc(deviceCount * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(Instance, &deviceCount, PhysicalDevices);
    physicalDevice = PhysicalDevices[0];

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, NULL);

    VkQueueFamilyProperties* queueFamilies = malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies);

    glfwCreateWindowSurface(Instance, window, NULL, &surface);

    int graphicsFamily = -1;
    int presentFamily = -1;
    for (int i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

        if (presentSupport) {
            presentFamily = i;
        }

        if (graphicsFamily != -1 && presentFamily != -1) {
            break;
        }
    }

    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo[2] = {};
    queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo[0].queueFamilyIndex = graphicsFamily;
    queueCreateInfo[0].queueCount = 1;
    queueCreateInfo[0].pQueuePriorities = &queuePriority;

    if (graphicsFamily != presentFamily) {
        queueCreateInfo[1].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo[1].queueFamilyIndex = presentFamily;
        queueCreateInfo[1].queueCount = 1;
        queueCreateInfo[1].pQueuePriorities = &queuePriority;
    }
    VkPhysicalDeviceFeatures deviceFeatures = {};

    VkDeviceCreateInfo DeviceCreateInfo = {};
    DeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    DeviceCreateInfo.pQueueCreateInfos = queueCreateInfo;
    DeviceCreateInfo.enabledExtensionCount = 1;
    DeviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;
    if (graphicsFamily != presentFamily) {
        DeviceCreateInfo.queueCreateInfoCount = 2;
    } else {
        DeviceCreateInfo.queueCreateInfoCount = 1;
    }
    DeviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    DeviceCreateInfo.enabledLayerCount = 1;
    DeviceCreateInfo.ppEnabledLayerNames = validationLayers;

    vkCreateDevice(physicalDevice, &DeviceCreateInfo, NULL, &device);

    vkGetDeviceQueue(device, graphicsFamily, 0, &graphicsQueue);
    vkGetDeviceQueue(device, presentFamily, 0, &presentQueue);

    VkSurfaceCapabilitiesKHR capabilities;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

    VkSurfaceFormatKHR* formats = NULL;
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, NULL);
    formats = malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats);

    VkPresentModeKHR* presentModes = NULL;
    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, NULL);
    presentModes = malloc(presentModeCount * sizeof(VkPresentModeKHR));
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes);

    VkSurfaceFormatKHR Format;
    for (int i = 0; i < formatCount; i++) {
        if (formats[i].format == VK_FORMAT_B8G8R8A8_SRGB && formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            Format = formats[i];
            break;
        }
    }

    VkPresentModeKHR PresentMode;
    for (int i = 0; i < presentModeCount; i++) {
        if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            PresentMode = presentModes[i];
        }
    }

    VkExtent2D Extent;
    if (capabilities.currentExtent.width != UINT32_MAX) {
        Extent = capabilities.currentExtent;
    } else {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = { width, height };

        clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
    }

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR SwapchaincreateInfo = {};
    SwapchaincreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    SwapchaincreateInfo.surface = surface;
    SwapchaincreateInfo.minImageCount = imageCount;
    SwapchaincreateInfo.imageFormat = Format.format;
    SwapchaincreateInfo.imageColorSpace = Format.colorSpace;
    SwapchaincreateInfo.imageExtent = Extent;
    SwapchaincreateInfo.imageArrayLayers = 1;
    SwapchaincreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = { graphicsFamily, presentFamily };

    if (graphicsFamily != presentFamily) {
        SwapchaincreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        SwapchaincreateInfo.queueFamilyIndexCount = 2;
        SwapchaincreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        SwapchaincreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        SwapchaincreateInfo.queueFamilyIndexCount = 0; // Optional
        SwapchaincreateInfo.pQueueFamilyIndices = NULL; // Optional
    }

    SwapchaincreateInfo.preTransform = capabilities.currentTransform;
    SwapchaincreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    SwapchaincreateInfo.presentMode = PresentMode;
    SwapchaincreateInfo.clipped = VK_TRUE;
    SwapchaincreateInfo.oldSwapchain = VK_NULL_HANDLE;

    vkCreateSwapchainKHR(device, &SwapchaincreateInfo, NULL, &swapChain);

    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, NULL);
    swapChainImages = malloc(imageCount * sizeof(VkImage));
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages);

    swapChainImageViews = malloc(imageCount * sizeof(VkImageView));

    for (size_t i = 0; i < imageCount; i++) {
        VkImageViewCreateInfo ViewCreateInfo = {};
        ViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        ViewCreateInfo.image = swapChainImages[i];
        ViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        ViewCreateInfo.format = Format.format;

        ViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        ViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        ViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        ViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        ViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        ViewCreateInfo.subresourceRange.baseMipLevel = 0;
        ViewCreateInfo.subresourceRange.levelCount = 1;
        ViewCreateInfo.subresourceRange.baseArrayLayer = 0;
        ViewCreateInfo.subresourceRange.layerCount = 1;

        vkCreateImageView(device, &ViewCreateInfo, NULL, &swapChainImageViews[i]);
    }

    FILE* FragmentFile = fopen("../res/compiled_shaders/example_frag.spv", "r");
    fseek(FragmentFile, 0L, SEEK_END);
    int ToAlloc = ftell(FragmentFile);
    char* FragData = malloc(ToAlloc * sizeof(char) + 1);
    fseek(FragmentFile, 0L, SEEK_SET);
    fread(FragData, 1, ToAlloc, FragmentFile);
    fclose(FragmentFile);

    VkShaderModuleCreateInfo FragmentShaderCreateInfo = {};
    FragmentShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    FragmentShaderCreateInfo.codeSize = ToAlloc;
    FragmentShaderCreateInfo.pCode = (uint32_t*)FragData;

    VkShaderModule FragmentShader;
    vkCreateShaderModule(device, &FragmentShaderCreateInfo, NULL, &FragmentShader);

    FILE* VertexFile
        = fopen("../res/compiled_shaders/example_vert.spv", "r");
    fseek(VertexFile, 0L, SEEK_END);
    ToAlloc = ftell(VertexFile);
    char* Vertdata = malloc(ToAlloc * sizeof(char) + 1);
    fseek(VertexFile, 0L, SEEK_SET);
    fread(Vertdata, 1, ToAlloc, VertexFile);
    fclose(VertexFile);

    VkShaderModuleCreateInfo VertexShaderCreateInfo = {};
    VertexShaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    VertexShaderCreateInfo.codeSize = ToAlloc;
    VertexShaderCreateInfo.pCode = (uint32_t*)Vertdata;

    VkShaderModule VertexShader;
    vkCreateShaderModule(device, &VertexShaderCreateInfo, NULL, &VertexShader);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    for (size_t i = 0; i < imageCount; i++) {
        vkDestroyImageView(device, swapChainImageViews[i], NULL);
    }

    vkDestroyShaderModule(device, FragmentShader, NULL);
    vkDestroyShaderModule(device, VertexShader, NULL);
    vkDestroySwapchainKHR(device, swapChain, NULL);
    vkDestroySurfaceKHR(Instance, surface, NULL);
    vkDestroyDevice(device, NULL);
    DestroyDebugUtilsMessengerEXT(Instance, debugMessenger, NULL);

    free(PhysicalDevices);
    free(queueFamilies);
    free(formats);
    free(presentModes);
}
