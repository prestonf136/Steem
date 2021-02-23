#include "Renderer.hpp"

void Steem::Renderer::SetDrawInfo(const Steem::RendererInfo& Info)
{
    m_InfoArr.push_back(Info);
}

void Steem::Renderer::Draw()
{
    for (auto i : m_InfoArr) {
        i.Shader->Bind();
        i.VertArr->Bind();
        i.IndexBuf->Bind();

        if (i.Callb != nullptr)
            i.Callb();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}