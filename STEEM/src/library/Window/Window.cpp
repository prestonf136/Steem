#include "Window.hpp"

Steem::Window::Window(Steem::WindowInfo  const &info)
{
    m_Info = info;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_Info.VersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_Info.VersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Info.Width, m_Info.Height, m_Info.Name.c_str(), nullptr, nullptr);
    ST_ASSERT(m_Window != nullptr);
    glfwMakeContextCurrent(m_Window);

    ST_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
};

void Steem::Window::AddRender(const Steem::Renderer &renderer)
{
    m_Renderers.push_back(renderer);
}

void Steem::Window::Bind() const
{
    while(!glfwWindowShouldClose(m_Window))
    {

        glfwPollEvents();
        glfwSwapBuffers(m_Window);

        glClearColor(m_Info.ClearColor.x,m_Info.ClearColor.y,m_Info.ClearColor.z, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(auto i : m_Renderers)
        {
            i.Draw();
        }
    }
}