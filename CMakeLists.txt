cmake_minimum_required(VERSION 3.9)
project(ColorCalibration)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED core imgproc highgui features2d)

set(SOURCE_FILES TransformationFinder.cpp TransformationFinder.h)
add_executable(opencv ${SOURCE_FILES} main.cpp )

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(opencv ${OpenCV_LIBS})