#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../library/steem_macros.hpp"
#include "../library/Shader/Shader.hpp"
#include "../library/VertexBuffer/VertexBuffer.hpp"
#include "../library/IndexBuffer/IndexBuffer.hpp"
#include "../library/VertexArray/VertexArray.hpp"
#include "../library/Renderer/Renderer.hpp"
#include "../library/Window/Window.hpp"

#include <iostream>

int main()
{
  const char *VertexSource = 
  "#version 330 core\n"
  "layout (location = 0) in vec4 aPos;\n"
  "layout (location = 1) in vec4 in_col;\n"
  "out vec4 color;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = aPos;\n"
  "   color = in_col;"
  "}\0";

  const char *FragSource = 
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "in vec4 color;\n"
  "uniform float u_Color;\n"
  "void main()\n"
  "{\n"
  "   FragColor = color;\n"
  "}\0";

  Steem::WindowInfo Winfo;
  Winfo.VersionMajor = 4;
  Winfo.VersionMinor = 6;
  Winfo.Height = 600;
  Winfo.Width = 800;
  Winfo.Name = "OpenGL";
  Winfo.ClearColor = glm::vec3(1.0f);

  Steem::Window win(Winfo);

  Steem::ShaderInfo ShadInf;
  ShadInf.FragmentData = FragSource;
  ShadInf.VertexData = VertexSource;

  Steem::Shader shad(ShadInf);

  Steem::VertexBufferInfo arr;

  GLfloat vertices[] = {
       0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
      -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
  };

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