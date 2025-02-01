cmake_minimum_required(VERSION 3.28)
project(image_processing)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(image_processing_lib
        include/SpatialDomainProcessor.h
        include/GrayscaleImageProcessor.h
        src/GrayscaleImageProcessor.cpp
        include/RGBImageProcessor.h
        src/RGBImageProcessor.cpp
        include/InputProcessor.h
        src/InputProcessor.cpp
        src/MorphologicalProcessor.cpp
        include/MorphologicalProcessor.h
        src/FourierProcessor.cpp
        include/FourierProcessor.h
        src/HistogramProcessor.cpp
        include/HistogramProcessor.h
        include/ImageComparer.cpp
        include/ImageComparer.h
)

target_link_libraries(image_processing_lib PUBLIC ${OpenCV_LIBS})
target_include_directories(image_processing_lib PUBLIC include)

add_executable(image_processing
        src/main.cpp
)

target_link_libraries(image_processing PRIVATE image_processing_lib)

enable_testing()

add_executable(image_processing_tests
        tests/GrayscaleImageProcessorTests.cpp
        tests/RGBImageProcessorTests.cpp
        tests/ImageProcessorTests.cpp
)

target_link_libraries(image_processing_tests
        PRIVATE
        image_processing_lib
        GTest::GTest
        GTest::Main
)

add_test(NAME ImageProcessingTests COMMAND image_processing_tests)
