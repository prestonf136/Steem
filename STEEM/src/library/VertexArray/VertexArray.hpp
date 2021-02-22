#include <glad/glad.h>
#include "../steem_macros.hpp"

namespace Steem {
class VertexArray
{
private:
    GLuint m_BufferID;
public:
    VertexArray();
    ~VertexArray();

    inline void Bind() const { glBindVertexArray(m_BufferID); };
    inline void UnBind() const { glBindVertexArray(0); };
};
}