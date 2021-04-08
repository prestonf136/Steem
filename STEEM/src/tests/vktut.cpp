#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include "../library/vendor/vk-bootstrap/src/VkBootstrap.h"
#include <GLFW/glfw3.h>

int main()
{
    glfwInit();

    VkInstance Instance;
    VkPhysicalDevice PhysicalDevice;
    VkDevice Device;

    VkQueue GraphicsQueue;
    VkQueue PresentQueue;
    VkSwapchainKHR SwapChain;

    GLFWwindow* Window = glfwCreateWindow(800, 600, "Hello Steem!", NULL, NULL);

    VkSurfaceKHR Surface;

    vkb::InstanceBuilder InstanceBuilder;
    auto InstanceReturn = InstanceBuilder.use_default_debug_messenger().request_validation_layers().build();

    Instance = InstanceReturn.value().instance;
    vkb::PhysicalDeviceSelector PhysicalDeviceSelector(InstanceReturn.value());

    glfwCreateWindowSurface(Instance, Window, NULL, &Surface);
    auto PhysicalDeviceReturn = PhysicalDeviceSelector.set_surface(Surface).select();

    PhysicalDevice = PhysicalDeviceReturn->physical_device;

    vkb::DeviceBuilder DeviceBuilder { PhysicalDeviceReturn.value() };
    auto DeviceBuilderRet = DeviceBuilder.build().value();
    Device = DeviceBuilderRet.device;

    vkb::SwapchainBuilder SwapChainBuilder { DeviceBuilderRet };
    SwapChain = SwapChainBuilder.build()->swapchain;

    GraphicsQueue = DeviceBuilderRet.get_queue(vkb::QueueType::graphics).value();
    PresentQueue = DeviceBuilderRet.get_queue(vkb::QueueType::present).value();

    while (!glfwWindowShouldClose(Window)) {
        glfwPollEvents();
    }
}