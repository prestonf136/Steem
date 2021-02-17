#include "VertexBuffer.hpp"

Steem::VertexBuffer::VertexBuffer(float *vertices)
{
  glGenBuffers(1, &m_ID);
  
  Bind();
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
};

Steem::VertexBuffer::~VertexBuffer()
{
  glDeleteBuffers(1, &m_ID);
};