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
        include/operations/ImageOperation.h
        include/operations/channel-operations/BrightnessOperation.h
        include/operations/channel-operations/ContrastLinearOperation.h
        include/operations/channel-operations/ContrastGammaOperation.h
        include/operations/channel-operations/NegativeOperation.h
        include/operations/channel-operations/FlipHorizontallyOperation.h
        include/operations/channel-operations/FlipVerticallyOperation.h
        include/operations/channel-operations/FlipDiagonallyOperation.h
        include/operations/channel-operations/ResizeOperation.h
        include/operations/channel-operations/MidpointFilterOperation.h
        include/operations/channel-operations/ArithmeticMeanFilterOperation.h
        include/operations/channel-operations/LaplacianFilterOperation.h
        include/operations/channel-operations/OptimizedLaplacianFilterOperation.h
        include/operations/channel-operations/RobertsOperation.h
        include/operations/whole-image-operations/RegionGrowingOperation.h
        src/input-processing-lib/OutputManager.cpp
        include/input-processing-lib/OutputManager.h
        include/operations/whole-image-operations/DilationOperation.h
        include/operations/whole-image-operations/ErosionOperation.h
        include/operations/whole-image-operations/OpeningOperation.h
        include/operations/whole-image-operations/ClosingOperation.h
        include/operations/whole-image-operations/HMTOperation.h
        include/operations/whole-image-operations/CompareImageStatsOperation.h
        include/operations/whole-image-operations/HistogramVisualizationOperation.h
        include/operations/channel-operations/HistogramEqualizationOperation.h
        include/operations/whole-image-operations/HistogramStatsOperation.h
        include/operations/channel-operations/FourierOperation.h
        include/operations/channel-operations/FastFourierOperation.h
        include/operations/channel-operations/LowPassOperation.h
        include/operations/channel-operations/HighPassOperation.h
        include/operations/channel-operations/BandPassOperation.h
        include/operations/channel-operations/BandCutOperation.h
        include/operations/channel-operations/PhaseShiftOperation.h
)

target_link_libraries(image_processing_lib PUBLIC ${OpenCV_LIBS})
target_include_directories(image_processing_lib PUBLIC include)

add_executable(image_processing
        src/main.cpp
)

target_link_libraries(image_processing PRIVATE image_processing_lib)

enable_testing()

add_executable(image_processing_tests
        tests/MorphologicalProcessorTests.cpp
        tests/SpatialDomainProcessorTests.cpp
        tests/ImageComparerTests.cpp
        tests/HistogramProcessorTests.cpp
)

target_link_libraries(image_processing_tests
        PRIVATE
        image_processing_lib
        GTest::GTest
        GTest::Main
)

add_test(NAME ImageProcessingTests COMMAND image_processing_tests)
