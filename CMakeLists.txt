cmake_minimum_required(VERSION 3.28)
project(image_processing)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(image_processing_lib
        include/image-processing-lib/SpatialDomainProcessor.h
        src/image-processing-lib/SpatialDomainProcessor.cpp
        include/input-processing-lib/InputProcessor.h
        src/input-processing-lib/InputProcessor.cpp
        src/image-processing-lib/MorphologicalProcessor.cpp
        include/image-processing-lib/MorphologicalProcessor.h
        src/image-processing-lib/FourierProcessor.cpp
        include/image-processing-lib/FourierProcessor.h
        src/image-processing-lib/HistogramProcessor.cpp
        include/image-processing-lib/HistogramProcessor.h
        src/image-processing-lib/ImageComparer.cpp
        include/image-processing-lib/ImageComparer.h
        include/input-processing-lib/Masks.h
        include/Constants.h
        include/input-processing-lib/CommandOptions.h
        src/input-processing-lib/CommandParser.cpp
        include/input-processing-lib/CommandParser.h
        include/input-processing-lib/CommandMapping.h
        include/ImageOperation.h
        include/channel-operations/BrightnessOperation.h
        include/channel-operations/ContrastLinearOperation.h
        include/channel-operations/ContrastGammaOperation.h
        include/channel-operations/NegativeOperation.h
        include/channel-operations/FlipHorizontallyOperation.h
        include/channel-operations/FlipVerticallyOperation.h
        include/channel-operations/FlipDiagonallyOperation.h
        include/channel-operations/ResizeOperation.h
        include/channel-operations/MidpointFilterOperation.h
        include/channel-operations/ArithmeticMeanFilterOperation.h
        include/whole-image-operations/LaplacianFilterOperation.h
        include/whole-image-operations/OptimizedLaplacianFilterOperation.h
        include/whole-image-operations/RobertsOperation.h
        include/whole-image-operations/RegionGrowingOperation.h
        src/input-processing-lib/OutputManager.cpp
        include/input-processing-lib/OutputManager.h
        include/whole-image-operations/DilationOperation.h
        include/whole-image-operations/ErosionOperation.h
        include/whole-image-operations/OpeningOperation.h
        include/whole-image-operations/ClosingOperation.h
        include/whole-image-operations/HMTOperation.h
        include/whole-image-operations/CompareImageStatsOperation.h
        include/whole-image-operations/HistogramVisualizationOperation.h
        include/channel-operations/HistogramEqualizationOperation.h
        include/whole-image-operations/HistogramStatsOperation.h
        include/channel-operations/FourierOperation.h
        include/channel-operations/FastFourierOperation.h
        include/channel-operations/LowPassOperation.h
        include/channel-operations/HighPassOperation.h
        include/channel-operations/BandPassOperation.h
        include/channel-operations/BandCutOperation.h
        include/channel-operations/PhaseShiftOperation.h
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
