#include "IndexBuffer.hpp"

Steem::IndexBuffer::IndexBuffer(Steem::IndexBufferInfo const &info)
{
    m_Info = info;
    glGenBuffers(1, &m_BufferID);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, info.size, info.IndexArray, GL_STATIC_DRAW);
}

Steem::IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_BufferID);
}

void Steem::IndexBuffer::SetAttrib(int size)
{
    Bind();
    glVertexAttribPointer(m_AttribPntr, size, GL_FLOAT, GL_FALSE, m_Info.stride, (void*)(m_Pad * sizeof(GLfloat)));
    glEnableVertexAttribArray(m_AttribPntr);
    
    m_AttribPntr++;
    used_pntrs.push_back(m_AttribPntr);
    m_Pad += size;
}