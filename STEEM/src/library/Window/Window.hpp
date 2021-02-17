#pragma once

#include <string>
#include <memory>
#include <iostream>
#include "../assert.hpp"

#include <GLFW/glfw3.h>

namespace Steem
{
  class Window
  {
    private:
      GLFWwindow* m_Window;
      std::string m_Name;

      uint32_t m_Width, m_Height;
    public:
      Window(const std::string &name, uint32_t width, uint32_t height, bool resizeable = false);
      ~Window();
  };
}
