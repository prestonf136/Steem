#pragma once

#include "../steem_macros.hpp"
#include <glad/glad.h>
#include <vector>

namespace Steem {
struct IndexBufferInfo {
    GLuint size;
    GLuint* IndexArray;
    GLuint stride = 0;
};
}

namespace Steem {
class IndexBuffer {
private:
    GLuint m_BufferID;
    GLuint m_AttribPntr = 0;
    GLuint m_Pad = 0;

    IndexBufferInfo m_Info;
    std::vector<GLuint> used_pntrs;

public:
    IndexBuffer(Steem::IndexBufferInfo const& info);
    ~IndexBuffer();

    void SetAttrib(int size);

    inline void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID); };
    inline void UnBind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); };
    inline int GetHighest() const { return used_pntrs.back(); };
};
}