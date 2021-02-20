#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../library/steem_macros.hpp"

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

  GLuint VertexShader;
  VertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(VertexShader, 1, &VertexSource, NULL);
  glCompileShader(VertexShader);

  int success;
  glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
  ST_ASSERT(success == true);

  const char *FragSource = 
  "#version 330 core\n"
  "out vec4 FragColor;\n"
  "in vec4 color;\n"
  "void main()\n"
  "{\n"
  "   FragColor = color;\n"
  "}\0";

  GLuint FragShader;
  FragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(FragShader, 1, &FragSource, NULL);
  glCompileShader(FragShader);

  glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
  ST_ASSERT(success == true);

  GLuint shaderProgram;
  shaderProgram = glCreateProgram();

  glAttachShader(shaderProgram, VertexShader);
  glAttachShader(shaderProgram, FragShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  ST_ASSERT(success == true);

  glDeleteShader(VertexShader);
  glDeleteShader(FragShader);

  GLfloat vertices[] = {
       0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
       0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
      -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
  };

  GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
  }; 

  ST_LOG(sizeof(vertices));
  GLuint VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
  while(!glfwWindowShouldClose(window))
  {
    glfwSwapBuffers(window);
    processInput(window);

    glClearColor(0.2f, 0.4f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, sizeof(vertices) / sizeof(GLfloat), GL_UNSIGNED_INT, 0);

    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
}