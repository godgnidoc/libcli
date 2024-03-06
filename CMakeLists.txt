cmake_minimum_required(VERSION 3.22)
project(cli)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)
add_compile_definitions(PROJECT_VERSION="$ENV{PROJECT_VERSION}")

include_directories(
    "${CMAKE_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/build/usr/lib)
install(DIRECTORY include/ DESTINATION ${CMAKE_SOURCE_DIR}/build/usr/include)

file(GLOB EXAMPLES "examples/*")
foreach(EXAMPLE ${EXAMPLES})
    if(NOT EXISTS ${EXAMPLE}/CMakeLists.txt)
        continue()
    endif()

    get_filename_component(EXAMPLE_NAME ${EXAMPLE} NAME)
    add_subdirectory(${EXAMPLE})
endforeach()