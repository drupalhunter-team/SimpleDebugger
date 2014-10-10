cmake_minimum_required(VERSION 2.8.4)
project(SimpleDebugger)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Symbols.cpp)
add_executable(SimpleDebugger ${SOURCE_FILES})
target_link_libraries(SimpleDebugger bfd)
