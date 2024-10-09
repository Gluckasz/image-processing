cmake_minimum_required(VERSION 3.28)
project(image_processing)

set(CMAKE_CXX_STANDARD 20)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(image_processing src/main.cpp
        src/GrayscaleImageProcessor.cpp
        include/ImageProcessor.h
        include/GrayscaleImageProcessor.h
        include/RGBImageProcessor.h
        src/RGBImageProcessor.cpp
)

target_link_libraries(image_processing ${OpenCV_LIBS})
