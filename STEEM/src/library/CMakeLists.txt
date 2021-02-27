add_subdirectory("vendor/glfw")

add_library(
  STEEM_LIBRARY
  INTERFACE
)

target_link_libraries(
  STEEM_LIBRARY INTERFACE 
  vulkan
  dl
  pthread
  X11
  Xxf86vm
  Xrandr
  Xi
  glfw
)
