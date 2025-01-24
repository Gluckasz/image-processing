//
// Created by gluckasz on 10/9/24.
//

#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#include <string>
#include <cstring>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <fstream>
#include <chrono>

#include "../include/ImageProcessor.h"
#include "../include/RGBImageProcessor.h"
#include "../include/GrayscaleImageProcessor.h"

constexpr int INPUT_IMAGE_POS = 1;

/**
 * @brief Processes command-line input for image manipulation operations.
 *
 * This class handles parsing and validation of command-line arguments,
 * manages image processing operations, and coordinates output generation.
 */
class InputProcessor {
public:
    /**
     * @brief Construct a new Input Processor object
     * @param argc Number of command-line arguments
     * @param argv Array of command-line argument strings
     */
    InputProcessor(int argc, char **argv);

    /**
     * @brief Process command-line arguments and set corresponding flags
     */
    void processInput();

    /**
     * @brief Execute image processing operations based on set flags
     */
    void processImage() const;

private:
    // Command-line input state
    int argc;
    char **argv;

    /**
     * @brief Output configuration
     */
    std::string outputFileName = "output.bmp";
    const std::string OUTPUT_DIR_NAME = "output";
    cv::ImreadModes imreadMode = cv::IMREAD_COLOR;

    /**
     * @brief Basic image transformation parameters
     */
    std::optional<int> brightnessModVal;
    std::optional<int> contrastLinearModVal;
    std::optional<float> contrastGammaModVal;
    bool isNegative = false;
    bool isHorizontalFlip = false;
    bool isVerticalFlip = false;
    bool isDiagonalFlip = false;
    std::optional<float> shrinkModVal;
    std::optional<float> enlargeModVal;

    /**
     * @brief Filter parameters
     */
    std::optional<int> midpointKernelSize;
    std::optional<int> arithmeticMeanKernelSize;

    /**
     * @brief Image comparison parameters
     */
    bool isNoNoise = false;
    std::string noNoiseImage;
    bool isMeanSquareError = false;
    bool isPeakMeanSquareError = false;
    bool isSignalToNoise = false;
    bool isPeakSignalToNoise = false;
    bool isMaximumDifference = false;

    /**
     * @brief Histogram and statistical parameters
     */
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

    /**
     * @brief Edge detection and morphological parameters
     */
    std::optional<int> laplaceMask;
    bool isOptimizedLaplacian = false;
    bool isRobertsOperator = false;
    std::optional<int> dilationMask;
    std::optional<int> erosionMask;
    std::optional<int> openingMask;
    std::optional<int> closingMask;
    std::optional<int> hmtMask;

    /**
     * @brief Advanced processing parameters
     */
    bool isTaskM4 = false;
    std::optional<int> regionGrowing;
    bool isFourierTransform = false;
    bool isFastFourierTransform = false;

    /**
     * @brief Frequency domain processing parameters
     */
    std::optional<int> lowPassBandSize;
    std::optional<int> highPassBandSize;
    bool isBandPass = false;
    std::optional<int> lowCut;
    std::optional<int> highCut;
    bool isBandCut = false;
    std::optional<int> lowPass;
    std::optional<int> highPass;
    std::optional<int> highPassDirectionMask;
    std::optional<int> taskF6k;
    std::optional<int> taskF6l;

    /**
     * @brief Command type enumeration for argument parsing
     */
    enum class CommandType {
        HELP,
        OUTPUT,
        GRAYSCALE,
        BRIGHTNESS,
        CONTRAST_LINEAR,
        CONTRAST_GAMMA,
        NEGATIVE,
        HORIZONTAL_FLIP,
        VERTICAL_FLIP,
        DIAGONAL_FLIP,
        SHRINK,
        ENLARGE,
        MIDPOINT_FILTER,
        ARITHMETIC_MEAN_FILTER,
        NO_NOISE_IMAGE,
        MSE,
        PMSE,
        SNR,
        PSNR,
        MAXIMUM_DIFFERENCE,
        HISTOGRAM,
        HISTOGRAM_UNIFORM,
        MEAN,
        VARIANCE,
        STANDARD_DEVIATION,
        VARIATION_1,
        ASYMMETRY,
        FLATTENING,
        VARIATION_2,
        ENTROPY,
        LAPLACE,
        OPTIMIZED_LAPLACE,
        ROBERTS_OPERATOR,
        DILATION,
        EROSION,
        OPENING,
        CLOSING,
        HMT,
        TASK_M4,
        REGION_GROWING,
        FOURIER_TRANSFORM,
        FAST_FOURIER_TRANSFORM,
        FFT_LOW_PASS,
        FFT_HIGH_PASS,
        FFT_BAND_PASS,
        FFT_BAND_CUT,
        FFT_HIGH_PASS_DIRECTION,
        FFT_PHASE_MODIFYING,
        UNKNOWN // For unrecognized commands
    };

    /**
     * @brief Maps command strings to CommandType enum values
     */
    const std::unordered_map<std::string, CommandType> commandMap = {
        {"--help", CommandType::HELP},
        {"--output", CommandType::OUTPUT},
        {"--grayscale", CommandType::GRAYSCALE},
        {"--brightness", CommandType::BRIGHTNESS},
        {"--contrastLinear", CommandType::CONTRAST_LINEAR},
        {"--contrastGamma", CommandType::CONTRAST_GAMMA},
        {"--negative", CommandType::NEGATIVE},
        {"--hflip", CommandType::HORIZONTAL_FLIP},
        {"--vflip", CommandType::VERTICAL_FLIP},
        {"--dflip", CommandType::DIAGONAL_FLIP},
        {"--shrink", CommandType::SHRINK},
        {"--enlarge", CommandType::ENLARGE},
        {"--mid", CommandType::MIDPOINT_FILTER},
        {"--amean", CommandType::ARITHMETIC_MEAN_FILTER},
        {"--noNoiseImage", CommandType::NO_NOISE_IMAGE},
        {"--mse", CommandType::MSE},
        {"--pmse", CommandType::PMSE},
        {"--snr", CommandType::SNR},
        {"--psnr", CommandType::PSNR},
        {"--md", CommandType::MAXIMUM_DIFFERENCE},
        {"--histogram", CommandType::HISTOGRAM},
        {"--huniform", CommandType::HISTOGRAM_UNIFORM},
        {"--cmean", CommandType::MEAN},
        {"--cvariance", CommandType::VARIANCE},
        {"--cstdev", CommandType::STANDARD_DEVIATION},
        {"--cvarcoi", CommandType::VARIATION_1},
        {"--casyco", CommandType::ASYMMETRY},
        {"--cflatco", CommandType::FLATTENING},
        {"--cvarcoii", CommandType::VARIATION_2},
        {"--centropy", CommandType::ENTROPY},
        {"--slaplace", CommandType::LAPLACE},
        {"--oslaplace", CommandType::OPTIMIZED_LAPLACE},
        {"--orobertsi", CommandType::ROBERTS_OPERATOR},
        {"--dilation", CommandType::DILATION},
        {"--erosion", CommandType::EROSION},
        {"--opening", CommandType::OPENING},
        {"--closing", CommandType::CLOSING},
        {"--hmt", CommandType::HMT},
        {"--taskM4", CommandType::TASK_M4},
        {"--regionGrowing", CommandType::REGION_GROWING},
        {"--fourierTransform", CommandType::FOURIER_TRANSFORM},
        {"--fastFourierTransform", CommandType::FAST_FOURIER_TRANSFORM},
        {"--fftLowPass", CommandType::FFT_LOW_PASS},
        {"--fftHighPass", CommandType::FFT_HIGH_PASS},
        {"--fftBandPass", CommandType::FFT_BAND_PASS},
        {"--fftBandCut", CommandType::FFT_BAND_CUT},
        {"--fftHighPassDirection", CommandType::FFT_HIGH_PASS_DIRECTION},
        {"--fftPhaseModifying", CommandType::FFT_PHASE_MODIFYING},
    };

    /**
     * @brief Maps CommandType enum values to their string representations
     */
    const std::unordered_map<CommandType, std::string> commandToStringMap = {
        {CommandType::HELP, "--help"},
        {CommandType::OUTPUT, "--output"},
        {CommandType::GRAYSCALE, "--grayscale"},
        {CommandType::BRIGHTNESS, "--brightness"},
        {CommandType::CONTRAST_LINEAR, "--contrastLinear"},
        {CommandType::CONTRAST_GAMMA, "--contrastGamma"},
        {CommandType::NEGATIVE, "--negative"},
        {CommandType::HORIZONTAL_FLIP, "--hflip"},
        {CommandType::VERTICAL_FLIP, "--vflip"},
        {CommandType::DIAGONAL_FLIP, "--dflip"},
        {CommandType::SHRINK, "--shrink"},
        {CommandType::ENLARGE, "--enlarge"},
        {CommandType::MIDPOINT_FILTER, "--mid"},
        {CommandType::ARITHMETIC_MEAN_FILTER, "--amean"},
        {CommandType::NO_NOISE_IMAGE, "--noNoiseImage"},
        {CommandType::MSE, "--mse"},
        {CommandType::PMSE, "--pmse"},
        {CommandType::SNR, "--snr"},
        {CommandType::PSNR, "--psnr"},
        {CommandType::MAXIMUM_DIFFERENCE, "--md"},
        {CommandType::HISTOGRAM, "--histogram"},
        {CommandType::HISTOGRAM_UNIFORM, "--huniform"},
        {CommandType::MEAN, "--cmean"},
        {CommandType::VARIANCE, "--cvariance"},
        {CommandType::STANDARD_DEVIATION, "--cstdev"},
        {CommandType::VARIATION_1, "--cvarcoi"},
        {CommandType::ASYMMETRY, "--casyco"},
        {CommandType::FLATTENING, "--cflatco"},
        {CommandType::VARIATION_2, "--cvarcoii"},
        {CommandType::ENTROPY, "--centropy"},
        {CommandType::LAPLACE, "--slaplace"},
        {CommandType::OPTIMIZED_LAPLACE, "--oslaplace"},
        {CommandType::ROBERTS_OPERATOR, "--orobertsi"},
        {CommandType::DILATION, "--dilation"},
        {CommandType::EROSION, "--erosion"},
        {CommandType::OPENING, "--opening"},
        {CommandType::CLOSING, "--closing"},
        {CommandType::HMT, "--hmt"},
        {CommandType::TASK_M4, "--taskM4"},
        {CommandType::REGION_GROWING, "--regionGrowing"},
        {CommandType::FOURIER_TRANSFORM, "--fourierTransform"},
        {CommandType::FAST_FOURIER_TRANSFORM, "--fastFourierTransform"},
        {CommandType::FFT_LOW_PASS, "--fftLowPass"},
        {CommandType::FFT_HIGH_PASS, "--fftHighPass"},
        {CommandType::FFT_BAND_PASS, "--fftBandPass"},
        {CommandType::FFT_BAND_CUT, "--fftBandCut"},
        {CommandType::FFT_HIGH_PASS_DIRECTION, "--fftHighPassDirection"},
        {CommandType::FFT_PHASE_MODIFYING, "--fftPhaseModifying"},
    };

    /**
     * @brief Print usage information and available commands
     */
    void printCommands();

    /**
     * @brief Template function to read and validate numeric parameters
     * @tparam T Parameter type (int or float)
     */
    template<typename T>
    bool readParam(const std::string &arg, const std::string &prefix, std::optional<T> &result,
                   const std::string &errorMsg);

    /**
     * @brief Read and validate string parameters
     */
    static bool readStringParam(const std::string &arg, const std::string &prefix, std::string &result,
                                const std::string &errorMsg);

    /**
     * @brief Apply selected image transformations to the input image
     */
    void applyImageTransformations(cv::Mat &image, std::unique_ptr<ImageProcessor> &imageProcessor) const;

    /**
     * @brief Calculate and save image comparison statistics
     */
    void calculateAndSaveComparisonImageStatistics(
        const cv::Mat &compareImage,
        const cv::Mat &originalImage,
        const cv::Mat &newImage,
        std::unique_ptr<ImageProcessor> &imageProcessor
    ) const;

    /**
     * @brief Calculate and save image statistics
     */
    void calculateAndSaveImageStats(
        const cv::Mat &newImage,
        std::unique_ptr<ImageProcessor> &imageProcessor
    ) const;

    /**
     * @brief Save processed image to output directory
     * @param image Image to save
     * @param outputFileName Name of output file
     */
    void saveImage(cv::Mat image, std::string outputFileName) const;

    /**
     * @brief Apply Fast Fourier Transform and save visualization
     * @param image Input image
     * @param fourierVisPath Path to save FFT visualization
     * @param imageProcessor Image processor instance
     * @return Transformed image
     */
    cv::Mat applyFastFourier(cv::Mat image, const std::string &fourierVisPath,
                             std::unique_ptr<ImageProcessor> &imageProcessor) const;
};


#endif //INPUTPROCESSOR_H
