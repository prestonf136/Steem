cmake_minimum_required(VERSION 3.10)

project(STEEM)

enable_testing()

add_subdirectory("STEEM/src/library")

include(CTest)

add_executable(WindowTest "STEEM/src/tests/window_test.cpp")
target_link_libraries(WindowTest PRIVATE STEEM_LIBRARY)
if(BUILD_TESTING)
  add_test(STEEM_TESTS WindowTest)
endif()
