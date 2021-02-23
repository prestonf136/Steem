add_subdirectory("vendor/glm")
add_subdirectory("vendor/glfw")
add_subdirectory("vendor/glad")

add_library(
  STEEM_LIBRARY
  INTERFACE
)

add_library(
  Shader
  Shader/Shader.cpp
  Shader/Shader.hpp
)

add_library(
  VertexBuffer
  VertexBuffer/VertexBuffer.cpp
  VertexBuffer/VertexBuffer.hpp
)

add_library(
  IndexBuffer
  IndexBuffer/IndexBuffer.cpp
  IndexBuffer/IndexBuffer.hpp
)

add_library(
  VertexArray
  VertexArray/VertexArray.cpp
  VertexArray/VertexArray.hpp
)

add_library(
  Renderer
  Renderer/Renderer.cpp
  Renderer/Renderer.hpp
)

add_library(
  Window
  Window/Window.cpp
  Window/Window.hpp
)

target_link_libraries(Shader PUBLIC glad glm::glm)
target_link_libraries(Renderer PUBLIC Shader VertexArray IndexBuffer glad glm::glm)
target_link_libraries(Window PUBLIC Renderer glm::glm glad glfw)
target_link_libraries(VertexBuffer PUBLIC glad)
target_link_libraries(IndexBuffer PUBLIC glad)
target_link_libraries(VertexArray PUBLIC glad)

target_link_libraries(
  STEEM_LIBRARY INTERFACE 
  Shader
  VertexBuffer
  IndexBuffer
  VertexArray
  Renderer
  Window

  glfw
  glad
  glm::glm
)
