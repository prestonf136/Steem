add_subdirectory("vendor/glfw")
add_subdirectory("vendor/glm")

find_package(Vulkan REQUIRED)

add_library(
  Window
  Window/Window.cpp
  Window/Window.hpp
)

add_library(
  STEEM_LIBRARY
  INTERFACE
)

target_link_libraries(STEEM_LIBRARY INTERFACE Vulkan::Vulkan Window glfw)
