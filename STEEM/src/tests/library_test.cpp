// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on
#include "../library/IndexBuffer/IndexBuffer.hpp"
#include "../library/Renderer/Renderer.hpp"
#include "../library/Shader/Shader.hpp"
#include "../library/VertexArray/VertexArray.hpp"
#include "../library/VertexBuffer/VertexBuffer.hpp"
#include "../library/Window/Window.hpp"
#include "../library/steem_macros.hpp"

#include <iostream>

int main()
{
    Steem::WindowInfo Winfo;
    Winfo.VersionMajor = 4;
    Winfo.VersionMinor = 6;
    Winfo.Height = 600;
    Winfo.Width = 800;
    Winfo.Name = "OpenGL";
    Winfo.ClearColor = glm::vec3(0.0f);

    Steem::Window win(Winfo);

    Steem::ShaderInfo ShadInf;
    ShadInf.FragmentData = Steem::ReadFromFile("res/shaders/example.frag");
    ShadInf.VertexData = Steem::ReadFromFile("res/shaders/example.vert");

    Steem::Shader shad(ShadInf);

    Steem::VertexBufferInfo arr;
    // clang-format off
    GLfloat vertices[] = {
        0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
       -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
       -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
    };
    // clang-format on
    arr.size = sizeof(vertices);
    arr.VertexArray = vertices;
    Steem::VertexBuffer buf(arr);

    Steem::VertexArray vao;
    vao.Bind();

    Steem::IndexBufferInfo ibo;

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    ibo.IndexArray = indices;
    ibo.size = sizeof(indices);
    ibo.stride = 5 * sizeof(GLfloat);

    Steem::IndexBuffer ib(ibo);
    ib.SetAttrib(2);
    ib.SetAttrib(3);

    Steem::RendererInfo renderInfo;
    renderInfo.IndexBuf = &ib;
    renderInfo.VertArr = &vao;
    renderInfo.Shader = &shad;

    renderInfo.Size = sizeof(indices) / sizeof(GLfloat);
    renderInfo.Vertices = vertices;

    Steem::Renderer renderer;
    renderer.SetDrawInfo(renderInfo);
    win.AddRender(renderer);

    win.Bind();
}