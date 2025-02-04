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
        src/channel-operations/BrightnessOperation.cpp
        src/channel-operations/ContrastLinearOperation.cpp
        src/channel-operations/ContrastGammaOperation.cpp
        src/channel-operations/NegativeOperation.cpp
        src/channel-operations/FlipHorizontallyOperation.cpp
        src/channel-operations/FlipVerticallyOperation.cpp
        src/channel-operations/FlipDiagonallyOperation.cpp
        src/channel-operations/ResizeOperation.cpp
        src/channel-operations/MidpointFilterOperation.cpp
        src/channel-operations/ArithmeticMeanFilterOperation.cpp
        src/whole-image-operations/LaplacianFilterOperation.cpp
        src/whole-image-operations/OptimizedLaplacianFilterOperation.cpp
        src/whole-image-operations/RobertsOperation.cpp
        src/whole-image-operations/RegionGrowingOperation.cpp
        src/OutputManager.cpp
        include/OutputManager.h
        src/morphological-operations/DilationOperation.cpp
        src/morphological-operations/ErosionOperation.cpp
        src/morphological-operations/OpeningOperation.cpp
        src/morphological-operations/ClosingOperation.cpp
        src/morphological-operations/HMTOperation.cpp
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
