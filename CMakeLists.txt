cmake_minimum_required(VERSION 3.22)
project(libcli)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(BUILD_TESTING)
  enable_testing()
  find_package(GTest CONFIG REQUIRED)

  file(GLOB_RECURSE TEST_SOURCES "cpp/test/*.cpp")
  add_executable(main EXCLUDE_FROM_ALL ${TEST_SOURCES})
  target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic -Werror)

  target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/cpp/include")
  target_link_libraries(main PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

  include(GoogleTest)
  gtest_discover_tests(main)
endif()