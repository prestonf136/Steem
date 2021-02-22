#include "Shader.hpp"

Steem::Shader::Shader(Steem::ShaderInfo const &info)
{
    GLuint VertexShader;
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    auto VertexData = info.VertexData.c_str();
    glShaderSource(VertexShader, 1, &VertexData, NULL);
    glCompileShader(VertexShader);

    int success;
    glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
    ST_ASSERT(success == true);

    GLuint FragShader;
    auto FragmentData = info.FragmentData.c_str();
    FragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragShader, 1, &FragmentData, NULL);
    glCompileShader(FragShader);

    glGetShaderiv(FragShader, GL_COMPILE_STATUS, &success);
    ST_ASSERT(success == true);

    m_ShaderID = glCreateProgram();
    
    glAttachShader(m_ShaderID, VertexShader);
    glAttachShader(m_ShaderID, FragShader);
    glLinkProgram(m_ShaderID);

    glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &success);
    ST_ASSERT(success == true);

    glDeleteShader(VertexShader);
    glDeleteShader(FragShader);
};

Steem::Shader::~Shader()
{
    glDeleteProgram(m_ShaderID);
}