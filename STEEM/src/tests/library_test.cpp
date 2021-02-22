#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../library/steem_macros.hpp"
#include "../library/Shader/Shader.hpp"
#include "../library/VertexBuffer/VertexBuffer.hpp"
#include "../library/IndexBuffer/IndexBuffer.hpp"
#include "../library/VertexArray/VertexArray.hpp"
#include <iostream>

void fb_callb(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
};

bool toggle = true;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
      glPolygonMode(GL_FRONT_AND_BACK, toggle ? GL_FILL : GL_LINE );
      toggle = !toggle;
    }
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
  ST_ASSERT(window != nullptr);
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, key_callback);

  ST_ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
  glfwSetFramebufferSizeCallback(window, fb_callb);

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
  "void main()\n"
  "{\n"
  "   FragColor = color;\n"
  "}\0";
{
  Steem::ShaderInfo ShadInf;
  ShadInf.FragmentData = FragSource;
  ShadInf.VertexData = VertexSource;

  Steem::Shader shad(ShadInf);

  GLfloat vertices[] = {
       0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
      -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
  };


  Steem::VertexBufferInfo arr;
  arr.size = sizeof(vertices);
  arr.VertexArray = vertices;

  GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
  }; 

  Steem::VertexArray vao;

  vao.Bind();
  Steem::VertexBuffer buf(arr);

  Steem::IndexBufferInfo ibo;
  ibo.IndexArray = indices;
  ibo.size = sizeof(indices);
  ibo.stride = 5 * sizeof(GLfloat);

  Steem::IndexBuffer ib(ibo);
  ib.SetAttrib(2, 0);
  ib.SetAttrib(3, 2);

  ST_LOG(ib.GetHighest());
  
  while(!glfwWindowShouldClose(window))
  {
    glfwSwapBuffers(window);
    processInput(window);

    glClearColor(0.2f, 0.4f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shad.Bind();
    vao.Bind();
    ib.Bind();
    glDrawElements(GL_TRIANGLES, sizeof(vertices) / sizeof(GLfloat), GL_UNSIGNED_INT, 0);

    glfwPollEvents();
  }
}
  glfwTerminate();
}