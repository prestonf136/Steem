#include <bits/stdint-uintn.h>
#include <cassert>
#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

PFN_vkCreateDebugReportCallbackEXT fvkCreateDebugReportCallbackEXT = nullptr;
PFN_vkDestroyDebugReportCallbackEXT fvkDestroyDebugReportCallbackEXT = nullptr;

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallb(
    VkDebugReportFlagsEXT Flags,
    VkDebugReportObjectTypeEXT ObjType,
    uint64_t SrcObj,
    size_t Location,
    int32_t MsgCode,
    const char* LayerPrefix,
    const char* Msg,
    void* UserData)
{
    /* VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT*/
    if (Flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT)
        std::cout << "[Debug Info]: ";
    if (Flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
        std::cout << "[Debug Warning]: ";
    if (Flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
        std::cout << "[Error Report]: ";
    if (Flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT)
        std::cout << "[Debug Report]: ";
    std::cout << std::endl;
    std::cout << "\tBy: " << LayerPrefix << "\n\t" << Msg << std::endl;
    return VK_FALSE;
}

int main()
{
    VkInstance m_Instance = nullptr;
    VkPhysicalDevice m_GPU = nullptr;
    VkDevice m_Device = nullptr;
    VkPhysicalDeviceProperties m_GpuProperties = {};
    VkDebugReportCallbackEXT m_DebugReport = nullptr;

    std::vector<const char*>
        m_InstanceLayers { "VK_LAYER_KHRONOS_validation" };
    std::vector<const char*> m_InstanceExstentions { VK_EXT_DEBUG_REPORT_EXTENSION_NAME };

    uint32_t m_GraphicsFamilyIndex = 0;

    VkApplicationInfo appInfo {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_2;
    appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.pApplicationName = "Hello Vulkan!";

    VkInstanceCreateInfo InstanceInfo {};
    InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    InstanceInfo.pApplicationInfo = &appInfo;
    InstanceInfo.enabledLayerCount = m_InstanceLayers.size();
    InstanceInfo.ppEnabledLayerNames = m_InstanceLayers.data();
    InstanceInfo.enabledExtensionCount = m_InstanceExstentions.size();
    InstanceInfo.ppEnabledExtensionNames = m_InstanceExstentions.data();

    auto res = vkCreateInstance(&InstanceInfo, nullptr, &m_Instance);
    assert(res == VK_SUCCESS && "Create Instance Failed");

    fvkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(m_Instance, "vkCreateDebugReportCallbackEXT"));
    fvkDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(m_Instance, "vkDestroyDebugReportCallbackEXT"));

    assert(fvkCreateDebugReportCallbackEXT != nullptr && fvkDestroyDebugReportCallbackEXT != nullptr);

    VkDebugReportCallbackCreateInfoEXT DebugCallbInfo {};
    DebugCallbInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    DebugCallbInfo.pfnCallback = VulkanDebugCallb;
    DebugCallbInfo.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
    fvkCreateDebugReportCallbackEXT(m_Instance, &DebugCallbInfo, nullptr, &m_DebugReport);

    {
        uint32_t gpuCount;
        vkEnumeratePhysicalDevices(m_Instance, &gpuCount, nullptr);
        std::vector<VkPhysicalDevice> DeviceList(gpuCount);

        vkEnumeratePhysicalDevices(m_Instance, &gpuCount, DeviceList.data());
        m_GPU = DeviceList[0];
        vkGetPhysicalDeviceProperties(m_GPU, &m_GpuProperties);
    }

    {
        uint32_t familyCount;
        vkGetPhysicalDeviceQueueFamilyProperties(m_GPU, &familyCount, nullptr);
        std::vector<VkQueueFamilyProperties> FamilyProperties(familyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_GPU, &familyCount, FamilyProperties.data());

        bool found = false;
        for (uint32_t i = 0; i < FamilyProperties.size(); i++) {
            if (FamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                found = true;
                m_GraphicsFamilyIndex = i;
            }
        }

        assert(found);
    }

    {
        uint32_t propertyCount;
        vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
        std::vector<VkLayerProperties> InstanceLayerProperties(propertyCount);
        vkEnumerateInstanceLayerProperties(&propertyCount, InstanceLayerProperties.data());
    }

    float QueuePriorities[] = {
        1.0f
    };

    VkDeviceQueueCreateInfo deviceQueueInfo {};
    deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueInfo.queueFamilyIndex = m_GraphicsFamilyIndex;
    deviceQueueInfo.queueCount = 1;
    deviceQueueInfo.pQueuePriorities = QueuePriorities;

    VkDeviceCreateInfo deviceInfo {};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &deviceQueueInfo;

    res = vkCreateDevice(m_GPU, &deviceInfo, nullptr, &m_Device);
    assert(res == VK_SUCCESS);
    /* end */
    vkDestroyDevice(m_Device, nullptr);
    m_Device = nullptr;

    fvkDestroyDebugReportCallbackEXT(m_Instance, m_DebugReport, nullptr);
    vkDestroyInstance(m_Instance, nullptr);
    m_Instance = nullptr;
}