cmake_minimum_required(VERSION 2.8.4)
project(SimpleDebugger)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(APPLE)
    include_directories("/opt/local/include")
    find_library(BFD_LIBRARY bfd /opt/local/lib)
else(APPLE)
    find_library(BFD_LIBRARY bfd /usr/lib)
endif(APPLE)

set(SOURCE_FILES main.cpp Symbols.cpp)
add_executable(SimpleDebugger ${SOURCE_FILES})
target_link_libraries(SimpleDebugger ${BFD_LIBRARY})
