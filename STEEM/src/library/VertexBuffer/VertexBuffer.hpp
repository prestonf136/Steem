#pragma once

#include <string>
#include <iostream>
#include "../assert.hpp"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Steem
{
  class VertexBuffer
  {
  private:
    GLuint m_ID;
  public:
    VertexBuffer(float *vertices);
    ~VertexBuffer();

    inline void Bind() const {glBindBuffer(GL_ARRAY_BUFFER, m_ID); };
    inline void UnBind() const {glBindBuffer(GL_ARRAY_BUFFER, 0); };
  };
}