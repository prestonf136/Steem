add_subdirectory("vendor/glm")
add_subdirectory("vendor/glfw")
add_subdirectory("vendor/glad")

add_library(
  STEEM_LIBRARY
  INTERFACE
)

target_link_libraries(
  STEEM_LIBRARY INTERFACE 

  glfw
  glad
  glm::glm
)
