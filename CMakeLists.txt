cmake_minimum_required(VERSION 2.8.4)
project(SimpleDebugger)

set(SOURCE_FILES main.cpp)
add_executable(SimpleDebugger ${SOURCE_FILES})
target_link_libraries(SimpleDebugger bfd)
