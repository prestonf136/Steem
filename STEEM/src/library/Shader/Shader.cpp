#include "Shader.hpp"

Steem::Shader::Shader(Steem::ShaderInfo const& info)
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

void Steem::Shader::SetUniformf(const std::string& name, GLfloat _data)
{
    GLint loc = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniform1f(loc, _data);
};

void Steem::Shader::SetUniformf(const std::string& name, glm::vec2 _data)
{
    GLint loc = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniform2f(loc, _data.x, _data.y);
};

void Steem::Shader::SetUniformf(const std::string& name, glm::vec3 _data)
{
    GLint loc = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniform3f(loc, _data.x, _data.y, _data.z);
};

void Steem::Shader::SetUniformf(const std::string& name, glm::vec4 _data)
{
    GLint loc = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniform4f(loc, _data.x, _data.y, _data.z, _data.w);
};

Steem::Shader::~Shader()
{
    glDeleteProgram(m_ShaderID);
}

std::string Steem::ReadFromFile(std::string const& filename)
{
    std::ifstream file(filename);
    return std::string((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
};