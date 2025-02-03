cmake_minimum_required(VERSION 3.28)
project(image_processing)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(image_processing_lib
        include/SpatialDomainProcessor.h
        src/SpatialDomainProcessor.cpp
        include/InputProcessor.h
        src/InputProcessor.cpp
        src/MorphologicalProcessor.cpp
        include/MorphologicalProcessor.h
        src/FourierProcessor.cpp
        include/FourierProcessor.h
        src/HistogramProcessor.cpp
        include/HistogramProcessor.h
        src/ImageComparer.cpp
        include/ImageComparer.h
        include/Masks.h
        include/Constants.h
        include/CommandOptions.h
        src/CommandParser.cpp
        include/CommandParser.h
        include/CommandMapping.h
        include/ImageOperation.h
        src/ChannelOperations/BrightnessOperation.cpp
        src/ChannelOperations/ContrastLinearOperation.cpp
        src/ChannelOperations/ContrastGammaOperation.cpp
        src/ChannelOperations/NegativeOperation.cpp
        src/ChannelOperations/FlipHorizontallyOperation.cpp
        src/ChannelOperations/FlipVerticallyOperation.cpp
        src/ChannelOperations/FlipDiagonallyOperation.cpp
        src/ChannelOperations/ResizeOperation.cpp
        src/ChannelOperations/MidpointFilterOperation.cpp
        src/ChannelOperations/ArithmeticMeanFilterOperation.cpp
        src/WholeImageOperations/LaplacianFilterOperation.cpp
        src/WholeImageOperations/OptimizedLaplacianFilterOperation.cpp
        src/WholeImageOperations/RobertsOperation.cpp
        src/WholeImageOperations/RegionGrowingOperation.cpp
        src/OutputManager.cpp
        include/OutputManager.h
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
