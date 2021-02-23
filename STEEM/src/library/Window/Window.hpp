#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "../steem_macros.hpp"
#include "../Renderer/Renderer.hpp"
#include "glm/fwd.hpp"

#include <vector>
#include <string>

namespace Steem {
struct WindowInfo
{
    int VersionMajor;
    int VersionMinor;
    int Height;
    int Width;

    glm::vec3 ClearColor;
    std::string Name;
};
}

namespace Steem {

class Window
{
private:
    Steem::WindowInfo m_Info;
    GLFWwindow *m_Window;
    std::vector<Renderer> m_Renderers;
public:
    Window(Steem::WindowInfo  const &info);
    inline ~Window() { glfwTerminate(); };
    void AddRender(Steem::Renderer const &renderer);
    void Bind() const;
};
}