#include "Window.hpp"

Steem::Window::Window(const std::string &name, uint32_t width, uint32_t height, bool resizeable)
{
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, (resizeable) ? GLFW_TRUE : GLFW_FALSE);

  m_Window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
}