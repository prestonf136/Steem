#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../library/assert.hpp"

#include <iostream>

void fb_callb(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
};

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
  ASSERT(window != nullptr);
  glfwMakeContextCurrent(window);

  ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));
  glfwSetFramebufferSizeCallback(window, fb_callb);

  const char *VertexSource = 
  "#version 330 core\n"
  "layout (location = 0) in vec4 aPos;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = aPos;\n"
  "}\0";

  GLuint VertexShader;
  VertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(VertexShader, 1, &VertexSource, NULL);
  glCompileShader(VertexShader);

  int success;
  glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
  ASSERT(success == true);

  const char *FragSource = 
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\0";

  GLuint FragShader;
  FragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(FragShader, 1, &FragSource, NULL);
  glCompileShader(FragShader);

  glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
  ASSERT(success == true);

  GLuint shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, VertexShader);
  glAttachShader(shaderProgram, FragShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  ASSERT(success == true);

  glDeleteShader(VertexShader);
  glDeleteShader(FragShader);

  float vertices[] = {
     -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      0.0f,  0.5f, 0.0f,
  }; 

  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  
  glBindVertexArray(0);
  while(!glfwWindowShouldClose(window))
  {
    glfwSwapBuffers(window);
    processInput(window);

    glClearColor(0.2f, 0.4f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
}