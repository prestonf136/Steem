cmake_minimum_required(VERSION 3.10)

project(STEEM)

enable_testing()

add_subdirectory("STEEM/src/library")

add_subdirectory("STEEM/vendor/glfw")
add_subdirectory("STEEM/vendor/glm")

add_subdirectory("STEEM/src/tests")
