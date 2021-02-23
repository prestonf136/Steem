#pragma once

#include <glad/glad.h>

#include "../IndexBuffer/IndexBuffer.hpp"
#include "../Shader/Shader.hpp"
#include "../VertexArray/VertexArray.hpp"
#include "../steem_macros.hpp"

#include <algorithm>
#include <functional>

namespace Steem {
struct RendererInfo {
    Steem::Shader* Shader = nullptr;
    Steem::VertexArray* VertArr = nullptr;
    Steem::IndexBuffer* IndexBuf = nullptr;
    GLfloat* Vertices;
    GLuint Size;

    std::function<void()> Callb;
};
}

namespace Steem {
class Renderer {
private:
    std::vector<RendererInfo> m_InfoArr;

public:
    Renderer() = default;
    void SetDrawInfo(Steem::RendererInfo const& Info);
    void Draw();
};
}