#pragma once

#include "../steem_macros.hpp"
#include <glad/glad.h>

namespace Steem {
struct VertexBufferInfo {
    GLuint size;
    GLfloat* VertexArray;
};
}

namespace Steem {
class VertexBuffer {
private:
    GLuint m_BufferID;

public:
    VertexBuffer(Steem::VertexBufferInfo info);
    ~VertexBuffer();

    inline void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_BufferID); };
    inline void UnBind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); };
};
}