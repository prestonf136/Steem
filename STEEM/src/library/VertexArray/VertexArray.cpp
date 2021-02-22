#include "VertexArray.hpp"

Steem::VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_BufferID);
    glBindVertexArray(m_BufferID);
}

Steem::VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_BufferID);
}