#include "VertexBuffer.hpp"

Steem::VertexBuffer::VertexBuffer(Steem::VertexBufferInfo info)
{
  glGenBuffers(1, &m_BufferID);
  Bind();
  glBufferData(GL_ARRAY_BUFFER, info.size, info.VertexArray, GL_STATIC_DRAW);
};

Steem::VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_BufferID);
};