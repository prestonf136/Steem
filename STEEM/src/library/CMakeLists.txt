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

target_link_libraries(Shader PUBLIC glad)
target_link_libraries(VertexBuffer PUBLIC glad)

target_link_libraries(
  STEEM_LIBRARY INTERFACE 
  Shader
  VertexBuffer

  glfw
  glad
  glm::glm
)
