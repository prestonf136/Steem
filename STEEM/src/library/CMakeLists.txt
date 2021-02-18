add_subdirectory("vendor/LLGL")

add_library(
  STEEM_LIBRARY
  INTERFACE
)

target_link_libraries(STEEM_LIBRARY INTERFACE LLGL)
