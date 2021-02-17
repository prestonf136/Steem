#pragma once

#include <string>
#include <iostream>
#include "../assert.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
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
      void Bind();
  };
}
