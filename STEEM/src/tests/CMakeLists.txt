include(CTest)

add_executable(WindowTest "window_test.cpp")

if(BUILD_TESTING)
  add_test(STEEM_TESTS WindowTest)
endif()