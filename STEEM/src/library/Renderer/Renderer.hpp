#pragma once

#include <glad/glad.h>

#include "../Shader/Shader.hpp"
#include "../VertexArray/VertexArray.hpp"
#include "../IndexBuffer/IndexBuffer.hpp"
#include "../steem_macros.hpp"

#include <functional>
#include <algorithm>

namespace Steem {
struct RendererInfo
{
    Steem::Shader *Shader = nullptr;
    Steem::VertexArray *VertArr = nullptr;
    Steem::IndexBuffer *IndexBuf = nullptr;
    GLfloat *Vertices;
    GLuint Size;

    std::function<void()> Callb;
};
}

namespace Steem {
class Renderer
{
private:
    std::vector<RendererInfo> m_InfoArr;
public:
    Renderer() = default;
    void SetDrawInfo(Steem::RendererInfo const &Info);
    void Draw();
};
}