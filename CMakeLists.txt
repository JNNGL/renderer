cmake_minimum_required(VERSION 3.27)
project(renderer)

set(CMAKE_CXX_STANDARD 20)

set(DEPS_DIR ${CMAKE_SOURCE_DIR}/deps)

include_directories(${CMAKE_SOURCE_DIR}/src)

include_directories(${DEPS_DIR}/stb)

file(GLOB_RECURSE CXX_SOURCE_FILES src/*.cpp)
add_executable(renderer ${CXX_SOURCE_FILES})
