#include "Window.hpp"

Steem::Window::Window(const std::string &name, uint32_t width, uint32_t height, bool resizeable) : m_Name(name), m_Width(width), m_Height(height)
{
  ASSERT(glfwInit());
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  glfwWindowHint(GLFW_RESIZABLE, resizeable ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


  m_Window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
  glfwMakeContextCurrent(m_Window);

  glewExperimental = GL_TRUE;
  glewInit();
}

Steem::Window::~Window()
{
    glfwTerminate();
    glfwDestroyWindow(m_Window);
}

void Steem::Window::StartLoop()
{

  while(!glfwWindowShouldClose(m_Window))
  {  
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(m_Window, GL_TRUE);
    
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
  }
}