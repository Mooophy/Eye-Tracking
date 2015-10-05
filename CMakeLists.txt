cmake_minimum_required(VERSION 3.3)
project(track)

find_package( OpenCV REQUIRED )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp)
add_executable(track ${SOURCE_FILES})

target_link_libraries( track ${OpenCV_LIBS} )