#pragma once

#include <glad/glad.h>
#include "../steem_macros.hpp"

#include <string>

namespace Steem {
struct ShaderInfo
{
    std::string VertexData;
    std::string FragmentData;
};
}

namespace Steem {

std::string ReadFromFile(std::string const &filename);

class Shader
{
private:
    GLuint m_ShaderID;
public:
    Shader(Steem::ShaderInfo  const &info);
    ~Shader();

    inline void Bind() const { glUseProgram(m_ShaderID); };
    inline void UnBind() const { glUseProgram(0); };
};
}