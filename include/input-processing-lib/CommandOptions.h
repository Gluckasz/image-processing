//
// Created by gluckasz on 2/3/25.
//

#ifndef COMMANDOPTIONS_H
#define COMMANDOPTIONS_H
#include <optional>
#include <opencv2/opencv.hpp>

#include "../Constants.h"

struct CommandOptions {
#pragma region Input/Output configuration parameters
    cv::ImreadModes imreadMode = cv::IMREAD_COLOR;
    std::optional<std::string> outputFileName = Constants::DEFAULT_OUTPUT_NAME;
    bool showHelp = false;
#pragma endregion

#pragma region Basic image transformation parameters
    std::optional<int> brightnessModVal;
    std::optional<int> contrastLinearModVal;
    std::optional<float> contrastGammaModVal;
    bool isNegative = false;
    bool isHorizontalFlip = false;
    bool isVerticalFlip = false;
    bool isDiagonalFlip = false;
    std::optional<float> shrinkModVal;
    std::optional<float> enlargeModVal;
#pragma endregion

#pragma region Spatial domain filtering parameters
    std::optional<int> midpointKernelSize;
    std::optional<int> arithmeticMeanKernelSize;
#pragma endregion

#pragma region Image comparison parameters
    std::optional<std::string> noNoiseImage;
    bool isMeanSquareError = false;
    bool isPeakMeanSquareError = false;
    bool isSignalToNoise = false;
    bool isPeakSignalToNoise = false;
    bool isMaximumDifference = false;
#pragma endregion

#pragma region Histogram and statistical parameters
    std::optional<int> histogramChannel;
    std::optional<int> histogramUniformGMax;
    std::optional<int> histogramUniformGMin;
    bool isMean = false;
    bool isVariance = false;
    bool isStandardDeviation = false;
    bool isVariation1 = false;
    bool isAsymmetry = false;
    bool isFlattening = false;
    bool isVariation2 = false;
    bool isEntropy = false;
#pragma endregion

#pragma region Edge detection parameters
    std::optional<int> laplaceMask;
    bool isOptimizedLaplacian = false;
    bool isRobertsOperator = false;
#pragma endregion

#pragma region Morphological parameters
    std::optional<int> dilationMask;
    std::optional<int> erosionMask;
    std::optional<int> openingMask;
    std::optional<int> closingMask;
    std::optional<int> hmtMask;
#pragma endregion

#pragma region Segmentation parameters
    std::optional<int> regionGrowing;
#pragma endregion

#pragma region Frequency domain processing parameters
    bool isFourierTransform = false;
    bool isFastFourierTransform = false;
    std::optional<int> lowPassBandSize;
    std::optional<int> highPassBandSize;
    std::optional<int> lowCut;
    std::optional<int> highCut;
    std::optional<int> lowPass;
    std::optional<int> highPass;
    std::optional<int> highPassDirectionMask;
    std::optional<int> taskF6k;
    std::optional<int> taskF6l;
#pragma endregion
};

#endif //COMMANDOPTIONS_H
