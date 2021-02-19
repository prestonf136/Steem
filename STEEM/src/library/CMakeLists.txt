add_subdirectory("vendor/LLGL")
add_subdirectory("vendor/glm")
add_library(
  STEEM_LIBRARY
  INTERFACE
)

target_link_libraries(STEEM_LIBRARY INTERFACE glm::glm LLGL)
