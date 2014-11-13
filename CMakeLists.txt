cmake_minimum_required(VERSION 2.8.4)
project(detection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE Main.cpp Frame.cpp Capture.cpp)
set(HEADER Frame.h Capture.h)
add_executable(detection ${SOURCE})

#add_executable(detection Main.cpp)


find_package(OpenCV REQUIRED )
target_link_libraries(detection ${OpenCV_LIBS} )