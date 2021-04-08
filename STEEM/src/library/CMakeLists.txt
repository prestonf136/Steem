add_subdirectory("vendor/glfw")
add_subdirectory("vendor/vk-bootstrap")

add_library(
  STEEM_LIBRARY
  INTERFACE
)

find_package(Vulkan REQUIRED)

target_link_libraries(
  STEEM_LIBRARY INTERFACE 
  Vulkan::Vulkan
  dl
  pthread
  X11
  Xxf86vm
  Xrandr
  Xi
  glfw
  vk-bootstrap::vk-bootstrap
)
