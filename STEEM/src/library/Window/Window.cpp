#include "Window.hpp"

Steem::Window::Window(const std::string &name, uint32_t width, uint32_t height, bool resizeable) : m_Name(name), m_Width(width), m_Height(height)
{
  ASSERT(glfwInit());

  glfwWindowHint(GLFW_RESIZABLE, resizeable ? GLFW_TRUE : GLFW_FALSE);
  m_Window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
}

Steem::Window::~Window()
{
    glfwTerminate();
    glfwDestroyWindow(m_Window);
}
