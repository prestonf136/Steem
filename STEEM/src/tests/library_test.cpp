#include "../library/Window/Window.hpp"
#include "../library/VertexBuffer/VertexBuffer.hpp"

int main()
{
  auto win = Steem::Window("Hola! soy dora",800,600, false);
  
  float verts[] = {
    0.5f, 0.5f,
   -0.5f, 0.0f,
    0.0f, 0.0f
  };
  
  Steem::VertexBuffer vb(verts);
  
  win.Bind();
  return 0;
};

