add_subdirectory("vendor/glfw")
add_subdirectory("vendor/glm")

find_package(GLEW REQUIRED)

add_library(
  Window
  Window/Window.cpp
  Window/Window.hpp
)

add_library(
  STEEM_LIBRARY
  INTERFACE
)

target_link_libraries(STEEM_LIBRARY INTERFACE GLEW::GLEW Window glfw)
