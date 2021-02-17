#pragma once

#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Steem
{
  class Window
  {
    private:
      GLFWwindow* m_Window;
    public:
      Window(const std::string &name, uint32_t width, uint32_t height, bool resizeable = false);
  };
}