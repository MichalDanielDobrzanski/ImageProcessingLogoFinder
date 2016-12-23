cmake_minimum_required(VERSION 3.6)
project(open_cv)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp Processing.h Processing.cpp Image.h ImageProcessing.cpp ImageProcessing.h ImageMoments.cpp ImageMoments.h)

add_executable(open_cv ${SOURCE_FILES})


include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(open_cv ${OpenCV_LIBS})