#include <stddef.h>
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
    VkFramebuffer* swapChainFramebuffers = NULL;
    VkRenderPass renderPass;
    VkCommandBuffer* commandBuffers = NULL;
    VkCommandPool commandPool;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    VkDevice device;
    GLFWwindow* window = NULL;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;

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

    FILE* VertexFile = fopen("../res/compiled_shaders/example_vert.spv", "r");
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

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = VertexShader;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = FragmentShader;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = NULL; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = NULL;

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)Extent.width;
    viewport.height = (float)Extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    VkOffset2D Offset;
    Offset.x = 0;
    Offset.y = 0;
    scissor.offset = Offset;
    scissor.extent = Extent;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f; // Optional
    multisampling.pSampleMask = NULL; // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE; // Optional

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = NULL; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = NULL; // Optional

    vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL, &pipelineLayout);

    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = Format.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    vkCreateRenderPass(device, &renderPassInfo, NULL, &renderPass);

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;

    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = NULL; // Optional
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = NULL;

    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, NULL, &graphicsPipeline);

    swapChainFramebuffers = malloc(imageCount * sizeof(VkFramebuffer));

    for (size_t i = 0; i < imageCount; i++) {
        VkImageView attachments[] = {
            swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = Extent.width;
        framebufferInfo.height = Extent.height;
        framebufferInfo.layers = 1;

        vkCreateFramebuffer(device, &framebufferInfo, NULL, &swapChainFramebuffers[i]);
    }

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsFamily;
    poolInfo.flags = 0;
    vkCreateCommandPool(device, &poolInfo, NULL, &commandPool);

    commandBuffers = malloc(imageCount * sizeof(VkCommandBuffer));

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)imageCount;

    vkAllocateCommandBuffers(device, &allocInfo, commandBuffers);

    for (size_t i = 0; i < imageCount; i++) {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = NULL; // Optional

        vkBeginCommandBuffer(commandBuffers[i], &beginInfo);

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapChainFramebuffers[i];

        VkOffset2D offset;
        offset.x = 0;
        offset.y = 0;

        renderPassInfo.renderArea.offset = offset;
        renderPassInfo.renderArea.extent = Extent;

        VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);
        vkCmdEndRenderPass(commandBuffers[i]);

        vkEndCommandBuffer(commandBuffers[i]);
    }

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    vkCreateSemaphore(device, &semaphoreInfo, NULL, &imageAvailableSemaphore);
    vkCreateSemaphore(device, &semaphoreInfo, NULL, &renderFinishedSemaphore);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        uint32_t imageIndex;
        vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);

        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR swapChains[] = { swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = NULL;

        vkQueuePresentKHR(presentQueue, &presentInfo);
        vkQueueWaitIdle(presentQueue);
    }

    vkDeviceWaitIdle(device);

    vkDestroySemaphore(device, renderFinishedSemaphore, NULL);
    vkDestroySemaphore(device, imageAvailableSemaphore, NULL);
    vkDestroyCommandPool(device, commandPool, NULL);
    for (size_t i = 0; i < imageCount; i++) {
        vkDestroyFramebuffer(device, swapChainFramebuffers[i], NULL);
    }

    vkDestroyPipeline(device, graphicsPipeline, NULL);
    vkDestroyRenderPass(device, renderPass, NULL);
    vkDestroyPipelineLayout(device, pipelineLayout, NULL);
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
