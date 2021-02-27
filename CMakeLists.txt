cmake_minimum_required(VERSION 3.10)

project(STEEM)
set (CMAKE_CXX_STANDARD 17)
enable_testing()

add_subdirectory("STEEM/src/library")

include(CTest)

add_executable(LIBRARY_TEST "STEEM/src/tests/library_test.cpp")
target_link_libraries(LIBRARY_TEST PRIVATE STEEM_LIBRARY)


if(BUILD_TESTING)
  add_test(STEEM_TESTS LIBRARY_TEST)
endif()
