#pragma once

#include "../steem_macros.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <string>

namespace Steem {
struct ShaderInfo {
    std::string VertexData;
    std::string FragmentData;
};
}

namespace Steem {

std::string ReadFromFile(std::string const& filename);

class Shader {
private:
    GLuint m_ShaderID;

public:
    Shader(Steem::ShaderInfo const& info);
    ~Shader();

    void SetUniformf(std::string const& name, glm::vec4 _data);
    void SetUniformf(std::string const& name, glm::vec3 _data);
    void SetUniformf(std::string const& name, glm::vec2 _data);
    void SetUniformf(std::string const& name, GLfloat _data);

    inline void Bind() const { glUseProgram(m_ShaderID); };
    inline void UnBind() const { glUseProgram(0); };
};
}