#include <LLGL/LLGL.h>
#include <memory>

int main() {
  std::unique_ptr<LLGL::RenderSystem>  Renderer = LLGL::RenderSystem::Load("OpenGL");

  LLGL::RenderContextDescriptor ContextDesc;
  ContextDesc.videoMode.resolution = {800, 600};
  ContextDesc.videoMode.fullscreen = false;
  ContextDesc.vsync.enabled = true;
  ContextDesc.samples = 8;
  LLGL::RenderContext *Context = Renderer->CreateRenderContext(ContextDesc);
  auto &window = LLGL::CastTo<LLGL::Window>(Context->GetSurface());

  window.SetTitle(L"STEEM engine library test");
  window.Show();

  struct TriangleVertex {
    float position[2];
    uint8_t color[4];
  };
  float s = 0.5f;
  TriangleVertex Triangle[] = {
      TriangleVertex{{0.0f, s}, {255, 0, 0, 255}},
      TriangleVertex{{s, -s}, {0, 255, 0, 255}},
      TriangleVertex{{-s, -s}, {0, 0, 255, 255}},
  };

  LLGL::VertexFormat VertexFormatAttrib;
  VertexFormatAttrib.AppendAttribute({"position", LLGL::Format::RG32Float});
  VertexFormatAttrib.AppendAttribute({"color", LLGL::Format::RGBA8UNorm});

  LLGL::BufferDescriptor BufferDescriptor;
  BufferDescriptor.size = sizeof(Triangle);
  BufferDescriptor.bindFlags = LLGL::BindFlags::VertexBuffer;
  BufferDescriptor.vertexAttribs = VertexFormatAttrib.attributes;
  LLGL::Buffer *myVertexBuffer =
      Renderer->CreateBuffer(BufferDescriptor, Triangle);

  LLGL::Shader *vertShader = nullptr;
  LLGL::Shader *fragShader = nullptr;
  LLGL::ShaderDescriptor vertShaderDesc, fragShaderDesc;
  vertShaderDesc = {LLGL::ShaderType::Vertex, "res/shaders/example.vert"};
  fragShaderDesc = {LLGL::ShaderType::Fragment, "res/shaders/example.frag"};

  vertShaderDesc.vertex.inputAttribs = VertexFormatAttrib.attributes;
  vertShader = Renderer->CreateShader(vertShaderDesc);
  fragShader = Renderer->CreateShader(fragShaderDesc);

  LLGL::ShaderProgramDescriptor shaderProgramDesc;
  {
    shaderProgramDesc.vertexShader = vertShader;
    shaderProgramDesc.fragmentShader = fragShader;
  }

  LLGL::ShaderProgram *ShaderProgram =
      Renderer->CreateShaderProgram(shaderProgramDesc);
  if (ShaderProgram->HasErrors())
    throw std::runtime_error(ShaderProgram->GetReport());

  LLGL::GraphicsPipelineDescriptor PipeLineDesc;
  PipeLineDesc.shaderProgram = ShaderProgram;
  PipeLineDesc.rasterizer.multiSampleEnabled = (ContextDesc.samples > 1);
  LLGL::PipelineState *PipeLine = Renderer->CreatePipelineState(PipeLineDesc);

  LLGL::CommandQueue *CmdQueue = Renderer->GetCommandQueue();
  LLGL::CommandBuffer *CmdBuffer = Renderer->CreateCommandBuffer();

  LLGL::Window &myWindow = LLGL::CastTo<LLGL::Window>(Context->GetSurface());

  while (myWindow.ProcessEvents()) {
    CmdBuffer->Begin();
    {
      CmdBuffer->SetViewport(Context->GetResolution());
      CmdBuffer->SetPipelineState(*PipeLine);
      CmdBuffer->SetVertexBuffer(*myVertexBuffer);
      
      CmdBuffer->BeginRenderPass(*Context);
      {
        CmdBuffer->Clear(LLGL::ClearFlags::Color);
        CmdBuffer->Draw(3, 0);
      }
      CmdBuffer->EndRenderPass();
    }
    CmdBuffer->End();

    CmdQueue->Submit(*CmdBuffer);
    Context->Present();
  }
};
